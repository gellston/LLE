import torch


class OnnxModel(torch.nn.Module):
    def __init__(self, backbone:torch.nn.Module):
        super(OnnxModel, self).__init__()

        self.backbone = backbone


    def forward(self, x):
        x, curve = self.backbone(x)

        return x