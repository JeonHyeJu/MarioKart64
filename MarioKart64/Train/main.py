from torch.nn import Linear, BatchNorm1d, PReLU, Module, MaxPool1d, Softmax, CrossEntropyLoss
import torch
import json
import numpy as np
from tqdm import tqdm
from torch.utils.data import Dataset, DataLoader
import torch.nn.functional as F
import torch.optim as optim

NUM_CLASSES = 3
BATCH = 128
EPOCH = 100
LR = 1e-2
DEVICE = "cuda:0"

class AverageMeter(object):
    """Computes and stores the average and current value"""

    def __init__(self):
        self.reset()

    def reset(self):
        self.val = 0
        self.avg = 0
        self.sum = 0
        self.count = 0

    def update(self, val, n=1):
        self.val = val
        self.sum += val * n
        self.count += n
        self.avg = self.sum / self.count

class Flatten(Module):
    def forward(self, input):
        return input.view(input.size(0), -1)

class MyDataset(Dataset):
    def __init__(self, path):
        with open(path, 'r') as f:
            self.data = json.load(f)

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        _data = self.data[idx]
        x = np.array([
            _data["initLoc"][0],
            _data["initLoc"][1],
            _data["forward"][0],
            _data["forward"][1],
            _data["forward"][2],
            _data["normX"],
            _data["normZ"],
            _data["rotY"],
            _data["isReverse"],
            _data["velocity"],
        ])

        # x = x.reshape(1, -1)
        y = np.array([
            self.data[idx]["dirV"],
            self.data[idx]["dirH"]
        ])
        # y = y.reshape(1, -1)

        return x, y
    
class Model(Module):
    def __init__(self, input_size=10):
        super(Model, self).__init__()

        self.fc1 = Linear(input_size, 64)
        self.fc2 = Linear(64, 128)
        self.fc3 = Linear(128, 512)
        self.fc4 = Linear(512, 128)
        self.max_pool = MaxPool1d(2)
        self.bn = BatchNorm1d(64)
        self.fc5 = Linear(64, 6)
        self.relu = PReLU()
        self.softmax = Softmax(2)
        self.flatten = Flatten()

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        x = self.relu(x)
        x = self.fc3(x)
        x = self.relu(x)
        x = self.fc4(x)
        x = self.relu(x)
        x = self.max_pool(x)
        x = self.bn(x)
        x = self.fc5(x)
        x = self.relu(x)
        x = x.view(-1, 2, 3)
        x = self.softmax(x)
        return x

def train():
    PATH = "C:\\Users\\JHJ\\Workspace\\1_Cpp\\3_MarioKart64\\MarioKart64\\APP\\train_data.txt"
    dataset = MyDataset(PATH)

    train_loader = DataLoader(dataset, batch_size=BATCH, shuffle=True, drop_last=True)
    # test_loader = DataLoader(test_data, batch_size=64, shuffle=True)

    model = Model()
    model.to(DEVICE)
    loss = CrossEntropyLoss()
    opt = optim.SGD(model.parameters(), lr=LR, momentum=0.9)

    # temp
    for epoch in range(EPOCH):
        losses = AverageMeter()

        for x, y in tqdm(train_loader):
            # print("shape:", x.shape, y.shape)
            x = x.to(DEVICE).float()
            y = y.to(DEVICE).long()
            pred = model(x)
            # pred = torch.argmax(pred, axis=2)
            pred_v = pred[:, 0, :]
            pred_h = pred[:, 1, :]
            y_v = y[:, 0]
            y_h = y[:, 1]
            loss_1 = loss(pred_v, y_v)
            loss_2 = loss(pred_h, y_h)
            last_loss = loss_1 + loss_2

            losses.update(last_loss.item(), x.size(0))

            opt.zero_grad()
            last_loss.backward()
            opt.step()

        epoch_loss = losses.avg
        print(f"{epoch}. loss avg: {epoch_loss:.2f}")

if __name__ == "__main__":
    train()