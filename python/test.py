import torch
import numpy as np
import cv2
import os


from model.zerodcepp import ZeroDCEPP


# GPU 사용 가능 여부 확인
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"현재 사용 중인 디바이스: {device}")



# Hyper Parameter
scale_factor = 4
num_features = 32
image_width = 1024
image_height = 1024
image_channel = 3

dataset_path = "C://github//dataset//lol_dataset//our485//low"
weight_path = "C://github//Dot4Seminar//working//python//results//weights.pth"
output_dir = "C://github//Dot4Seminar//working//python//results//output_images"


dummy_input = torch.randn(size=(1, image_channel, image_height, image_width)).to(device)

model = ZeroDCEPP(scale_factor=scale_factor, num_features=num_features)
model = model.to(device)
if os.path.exists(weight_path):
    state_dict = torch.load(weight_path, map_location=device)
    model.load_state_dict(state_dict)

model.eval()

os.makedirs(output_dir, exist_ok=True)

file_list = os.listdir(dataset_path)
image_extensions = ('.jpg', '.jpeg', '.png', '.bmp')


for file_name in file_list:
    if not file_name.lower().endswith(image_extensions):
        continue

    full_path = os.path.join(dataset_path, file_name)
    input = cv2.imread(full_path)
    
    if input is None:
        continue

    h, w, _ = input.shape

    input_img = cv2.cvtColor(input, cv2.COLOR_BGR2RGB)
    input_img = cv2.resize(input_img, (image_width, image_height))
    input_img = input_img.astype(np.float32) / 255.0
    input_img = np.transpose(input_img, (2, 0, 1))
    
    input_tensor = torch.from_numpy(input_img).unsqueeze(0).to(device)

    with torch.no_grad():
        enhanced_tensor, _ = model(input_tensor)


    output = enhanced_tensor.squeeze(0).cpu().numpy()
    output = np.transpose(output, (1, 2, 0))
    output = (output * 255.0).clip(0, 255).astype(np.uint8)
    
    output = cv2.cvtColor(output, cv2.COLOR_RGB2BGR)
    output = cv2.resize(output, (w, h))


    save_path = os.path.join(output_dir, file_name)
    cv2.imwrite(save_path, output)


    cv2.imshow('input', input)
    cv2.imshow('output', output)
    

    cv2.waitKey(1)