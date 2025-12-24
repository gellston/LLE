import torch
import torch.onnx
import numpy as np
import cv2
import os

from model.zerodcepp import ZeroDCEPP
from loss.zerodce_loss import ZeroDCETotalLoss
from dataset.lle_dataset import get_lle_loader
from model.onnx_model import OnnxModel

# GPU 사용 가능 여부 확인
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"현재 사용 중인 디바이스: {device}")

# Hyper Parameter
epochs = 50
learning_rate = 0.0001
weight_decay = 0.0001
batch_size = 8
scale_factor = 4
num_features = 32
image_width = 1024
image_height = 1024
image_channel = 3

spa_weight=1.0
exp_patch_size=16
exp_target_mean=0.5
col_weight=10.0
tv_weight=200.0

dataset_path = "C://github//dataset//lol_dataset//our485//all"
weight_path = "C://github//Dot4Seminar//working//python//results//weights.pth"
onnx_model_path = "C://github//Dot4Seminar//working//python//results//model.onnx"


dummy_input = torch.randn(size=(1, image_channel, image_height, image_width)).to(device)

model = ZeroDCEPP(scale_factor=scale_factor, num_features=num_features)
model = model.to(device)
if os.path.exists(weight_path):
    state_dict = torch.load(weight_path, map_location=device)
    model.load_state_dict(state_dict)


dataloader = get_lle_loader(dataset_path, batch_size, resize_shape=(image_height, image_width))
total_batches = len(dataloader)


loss = ZeroDCETotalLoss(spa_weight=spa_weight,
                        exp_patch_size=exp_patch_size,
                        exp_target_mean=exp_target_mean,
                        col_weight=col_weight,
                        tv_weight=tv_weight).to(device)
optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate, weight_decay=weight_decay)

temp_loss = 1000000
for epoch in range(epochs):
    avg_loss = 0

    model.train()
    for i, x_image in enumerate(dataloader):

        gpu_x_image = x_image.to(device)
        enhanced_img, curve_params = model(gpu_x_image)
        current_loss = loss(gpu_x_image, enhanced_img, curve_params)

        optimizer.zero_grad()
        current_loss.backward()
        optimizer.step()

        avg_loss += current_loss.item() / total_batches

    tensor_input_check = gpu_x_image[0]
    tensor_output_check = enhanced_img[0]
    # 1. CPU 이동 및 Numpy 변환
    input = tensor_input_check.detach().cpu().numpy()
    input = np.transpose(input, (1, 2, 0))
    input = (input * 255).astype(np.uint8)
    input = cv2.cvtColor(input, cv2.COLOR_RGB2BGR)

    # 1. CPU 이동 및 Numpy 변환
    output = tensor_output_check.detach().cpu().numpy()
    output = np.transpose(output, (1, 2, 0))
    output = (output * 255).astype(np.uint8)
    output = cv2.cvtColor(output, cv2.COLOR_RGB2BGR)


    # 5. 시각화
    cv2.imshow("original", input)
    cv2.imshow("output", output)
    cv2.waitKey(1) # 1ms 대기 (학습 루프 멈춤 방지)

    if temp_loss > avg_loss:
        temp_loss = avg_loss
        
        model.eval()
    
        onnx_model = OnnxModel(backbone=model)

        torch.save(model.state_dict(), weight_path)
        torch.onnx.export(
            onnx_model,                      # 실행할 모델
            dummy_input,                # 모델 입력 예시
            onnx_model_path,            # 저장 파일명
            export_params=True,         # 모델 파일 안에 학습된 파라미터 저장
            opset_version=11,           # Bilinear 연산을 안정적으로 지원하는 버전
            do_constant_folding=True,   # 상수 폴딩 최적화 (속도 향상)
            input_names=['input'],      # 입력 노드 이름 (C++에서 호출 시 사용)
            output_names=['output'],    # 출력 노드 이름
        )
        
    print('current avg loss = ', avg_loss)



