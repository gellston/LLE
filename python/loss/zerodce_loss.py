import torch
import torch.nn as nn
import torch.nn.functional as F

class ColorConstancyLoss(nn.Module):
    """L_color: RGB 채널 간의 불균형을 최소화하여 색상 왜곡 방지"""
    def __init__(self):
        super(ColorConstancyLoss, self).__init__()

    def forward(self, x):
        mean_rgb = torch.mean(x, [2, 3], keepdim=True)
        mean_r, mean_g, mean_b = torch.split(mean_rgb, 1, dim=1)
        
        diff_rg = torch.pow(mean_r - mean_g, 2)
        diff_rb = torch.pow(mean_r - mean_b, 2)
        diff_gb = torch.pow(mean_g - mean_b, 2)
        
        # 원색 편향성을 억제하는 k값 계산
        color_stability = torch.pow(torch.pow(diff_rg, 2) + torch.pow(diff_rb, 2) + torch.pow(diff_gb, 2), 0.5)
        return color_stability.mean()

class SpatialConstancyLoss(nn.Module):
    """L_spa: 인접 픽셀 간의 관계(대비)를 유지하여 디테일 보존"""
    def __init__(self):
        super(SpatialConstancyLoss, self).__init__()
        self.pool = nn.AvgPool2d(4)

    def forward(self, org, enhanced):
        device = org.device
        
        # 미분 커널 설정 (소문자 및 명확한 변수명으로 변경)
        kernel_left  = torch.FloatTensor([[0,0,0], [-1,1,0], [0,0,0]]).to(device).unsqueeze(0).unsqueeze(0)
        kernel_right = torch.FloatTensor([[0,0,0], [0,1,-1], [0,0,0]]).to(device).unsqueeze(0).unsqueeze(0)
        kernel_up    = torch.FloatTensor([[0,-1,0], [0,1,0],  [0,0,0]]).to(device).unsqueeze(0).unsqueeze(0)
        kernel_down  = torch.FloatTensor([[0,0,0], [0,1,0],  [0,-1,0]]).to(device).unsqueeze(0).unsqueeze(0)

        org_mean = torch.mean(org, 1, keepdim=True)
        enhanced_mean = torch.mean(enhanced, 1, keepdim=True)

        org_pool = self.pool(org_mean)
        enhanced_pool = self.pool(enhanced_mean)

        # 각 방향별 변화량(Gradient) 추출
        grad_org_l = F.conv2d(org_pool, kernel_left, padding=1)
        grad_org_r = F.conv2d(org_pool, kernel_right, padding=1)
        grad_org_u = F.conv2d(org_pool, kernel_up, padding=1)
        grad_org_d = F.conv2d(org_pool, kernel_down, padding=1)

        grad_enh_l = F.conv2d(enhanced_pool, kernel_left, padding=1)
        grad_enh_r = F.conv2d(enhanced_pool, kernel_right, padding=1)
        grad_enh_u = F.conv2d(enhanced_pool, kernel_up, padding=1)
        grad_enh_d = F.conv2d(enhanced_pool, kernel_down, padding=1)

        # 원본과 개선 이미지 간의 지역적 변화량 차이 계산
        loss_spatial = torch.pow(grad_org_l - grad_enh_l, 2) + torch.pow(grad_org_r - grad_enh_r, 2) + torch.pow(grad_org_u - grad_enh_u, 2) + torch.pow(grad_org_d - grad_enh_d, 2)
        
        return loss_spatial.mean()

class ExposureControlLoss(nn.Module):
    """L_exp: 이미지의 평균 밝기가 목표치(target_intensity)에 도달하도록 제어"""
    def __init__(self, patch_size, target_intensity=0.6):
        super(ExposureControlLoss, self).__init__()
        self.pool = nn.AvgPool2d(patch_size)
        self.target_intensity = target_intensity

    def forward(self, x):
        x = torch.mean(x, 1, keepdim=True)
        mean_map = self.pool(x)
        return torch.mean(torch.pow(mean_map - self.target_intensity, 2))

class TotalVariationLoss(nn.Module):
    """L_TV: 곡선 파라미터(x_r)의 급격한 변화를 억제하여 매끄러운 결과 생성"""
    def __init__(self, weight=1):
        super(TotalVariationLoss, self).__init__()
        self.weight = weight

    def forward(self, x):
        batch_size = x.size()[0]
        h_x = x.size()[2]
        w_x = x.size()[3]
        
        count_h = (h_x - 1) * w_x
        count_w = h_x * (w_x - 1)
        
        h_tv = torch.pow((x[:, :, 1:, :] - x[:, :, :h_x-1, :]), 2).sum()
        w_tv = torch.pow((x[:, :, :, 1:] - x[:, :, :, :w_x-1]), 2).sum()
        
        return self.weight * 2 * (h_tv / count_h + w_tv / count_w) / batch_size
    


class ZeroDCETotalLoss(nn.Module):
    def __init__(self, 
                 spa_weight=1.0, 
                 exp_patch_size=16, 
                 exp_target_mean=0.6, 
                 col_weight=10.0, 
                 tv_weight=200.0):
        super(ZeroDCETotalLoss, self).__init__()
        
        self.spatial_loss = SpatialConstancyLoss()
        self.exposure_loss = ExposureControlLoss(patch_size=exp_patch_size, target_intensity=exp_target_mean)
        self.color_loss = ColorConstancyLoss()
        self.tv_loss = TotalVariationLoss(weight=tv_weight)
        
        self.spa_weight = spa_weight
        self.col_weight = col_weight

    def forward(self, original, enhanced, curve_params):
        loss_s = self.spatial_loss(original, enhanced)
        loss_e = self.exposure_loss(enhanced)
        loss_c = self.color_loss(enhanced)
        loss_t = self.tv_loss(curve_params)
        
        total_loss = (self.spa_weight * loss_s) + loss_e + (self.col_weight * loss_c) + loss_t
                     
        return total_loss