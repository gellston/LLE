import torch
import torch.nn as nn
import torch.nn.functional as F



from model.block import DepthwiseSeparableConv


class ZeroDCEPP(nn.Module):
    def __init__(self, scale_factor=4, num_features=32):
        super(ZeroDCEPP, self).__init__()

        self.relu = nn.ReLU(inplace=True)
        self.scale_factor = scale_factor
        self.upsample = nn.UpsamplingBilinear2d(scale_factor=self.scale_factor)

        self.e_conv1 = DepthwiseSeparableConv(3, num_features) 
        self.e_conv2 = DepthwiseSeparableConv(num_features, num_features) 
        self.e_conv3 = DepthwiseSeparableConv(num_features, num_features) 
        self.e_conv4 = DepthwiseSeparableConv(num_features, num_features) 
        self.e_conv5 = DepthwiseSeparableConv(num_features * 2, num_features) # x3 + x4
        self.e_conv6 = DepthwiseSeparableConv(num_features * 2, num_features) # x2 + x5
        self.e_conv7 = DepthwiseSeparableConv(num_features * 2, 3)            # x1 + x6

    def apply_curve(self, x, x_r):
        for _ in range(8):
            x = x + x_r * (torch.pow(x, 2) - x)
        return x

    def forward(self, x):
        # 다운 스케일
        if self.scale_factor == 1:
            x_down = x
        else:
            x_down = F.interpolate(x, scale_factor=1/self.scale_factor, mode='bilinear', align_corners=False)


        # 특징 추출 레이어 시작 
        layer1 = self.relu(self.e_conv1(x_down))
        layer2 = self.relu(self.e_conv2(layer1))
        layer3 = self.relu(self.e_conv3(layer2))
        layer4 = self.relu(self.e_conv4(layer3))
        
        # Feature Concat
        layer5 = self.relu(self.e_conv5(torch.cat([layer3, layer4], 1)))
        layer6 = self.relu(self.e_conv6(torch.cat([layer2, layer5], 1)))
        layer7 = self.e_conv7(torch.cat([layer1, layer6], 1))
        

        x_r = torch.tanh(layer7)

        # 원본 사이즈로 다시 upsampling
        if self.scale_factor != 1:
            x_r = self.upsample(x_r)
        
        # 원본 이미지에 라이트맵을 곱함.
        enhance_image = self.apply_curve(x, x_r)
        
        return enhance_image, x_r