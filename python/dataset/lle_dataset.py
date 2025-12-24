import os
from PIL import Image
import torch
from torch.utils.data import Dataset, DataLoader
import torchvision.transforms as transforms

class LLEDataset(Dataset):
    def __init__(self, image_dir, transform=None):
        """
        Args:
            image_dir (str): 이미지 폴더 경로 (한글 경로 지원)
            transform (callable): torchvision.transforms 등 이미지 변환 함수
        """
        # 한 폴더 내의 모든 .png 파일 리스트업
        self.image_paths = [os.path.join(image_dir, f) for f in os.listdir(image_dir) 
                            if f.lower().endswith('.png')]
        self.transform = transform

    def __len__(self):
        return len(self.image_paths)

    def __getitem__(self, idx):
        img_path = self.image_paths[idx]
        
        # 1. 한글 경로 인식을 위한 처리
        # PIL은 open() 시 경로에 한글이 있어도 기본적으로 잘 작동합니다.
        with open(img_path, 'rb') as f:
            image = Image.open(f).convert('RGB')
        
        # 2. 전처리 및 증강 (외부에서 주입받은 transform 실행)
        if self.transform:
            image = self.transform(image)
        
        return image

# 2. 실행 및 Transform 정의 부분
def get_lle_loader(image_dir, batch_size=8, resize_shape=(512, 512)):
    # PyTorch 기본 제공 transform 구성
    combined_transform = transforms.Compose([
        transforms.Resize(resize_shape),            # 리사이즈
        transforms.RandomHorizontalFlip(p=0.5),    # 좌우 플립 (50% 확률)
        transforms.ToTensor(),                     # 0~1 정규화 및 Tensor 변환 (CHW)
    ])

    dataset = LLEDataset(
        image_dir=image_dir, 
        transform=combined_transform
    )
    
    return DataLoader(dataset, batch_size=batch_size, shuffle=True)