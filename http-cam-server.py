import httpx
import asyncio
from PIL import Image
from io import BytesIO
import os
from ultralytics import YOLO


class Controller:
    def __init__(self, model_name: str):
        self.client: httpx.AsyncClient = httpx.AsyncClient()
        self.model: YOLO = YOLO(model_name)
        self.count = 1

    async def set_640_480(self):
        await self.client.get("http://192.168.171.113/control?var=framesize&val=8")
        await self.client.get("http://192.168.171.113/xclk?xclk=20")
    
    def clean_cuda_cache(self):
        import torch

        os.environ['PYTORCH_CUDA_ALLOC_CONF'] = 'expandable_segments:True'
        torch.cuda.empty_cache()
    
    async def run_model(self, image: Image):
        results = self.model(image)  # return a list of Results objects

        for result in results:
            boxes = result.boxes  # Boxes object for bounding box outputs
            masks = result.masks  # Masks object for segmentation masks outputs
            keypoints = result.keypoints  # Keypoints object for pose outputs
            probs = result.probs  # Probs object for classification outputs
            obb = result.obb  # Oriented boxes object for OBB outputs
            result.show()  # display to screen
            # result.save(filename=f"result_{self.count}.jpg")  # save to disk
            await asyncio.sleep(2)
            result.exit()
            # self.count += 1
    
    async def get_frame(self) -> bytes:
        response = await self.client.get("http://192.168.171.113/capture", timeout=20)
        return response.content

    def transform_bytes_image(self, frame: bytes) -> Image:
        image = Image.open(BytesIO(frame))
        return image
        
    async def work(self):
        await self.set_640_480()
        self.clean_cuda_cache()
        while True:
            frame = await self.get_frame()
            image = self.transform_bytes_image(frame)
            await self.run_model(image)
        

async def main():
    controller = Controller("yolov8n.pt")
    await controller.work()



if __name__ == "__main__":
    asyncio.run(main())
