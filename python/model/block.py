
import torch


class DepthwiseSeparableConv(torch.nn.Module):
    def __init__(self, in_ch, out_ch):
        super(DepthwiseSeparableConv, self).__init__()
        self.depth_conv = torch.nn.Conv2d(in_ch, in_ch, kernel_size=3, padding=1, groups=in_ch)
        self.point_conv = torch.nn.Conv2d(in_ch, out_ch, kernel_size=1)

    def forward(self, x):
        return self.point_conv(self.depth_conv(x))
