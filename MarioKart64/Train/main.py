from torch.nn import Linear, BatchNorm1d, PReLU, Module, MaxPool1d, Softmax, CrossEntropyLoss, MSELoss
import torch
from sklearn import svm
import json
import numpy as np
from tqdm import tqdm
from torch.utils.data import Dataset, DataLoader
import torch.nn.functional as F
import torch.optim as optim

NUM_CLASSES = 3
BATCH = 128
EPOCH = 200
LR = 1e-4
DEVICE = "cuda:0"

def modulo(ys):
    if type(ys) == float or type(ys) == int:
        y = abs(ys)
        s = -1 if ys < 0 else 1
        v = y // 360
        y = y - v * 360
        ys = y * s
        ys /= 360
        return ys

    for i in range(ys.shape[0]):
        y = abs(ys[i])
        s = -1 if ys[i] < 0 else 1
        v = y // 360
        y = y - v * 360
        ys[i] = y * s
        ys[i] /= 360
    return ys
    
def getData(paths):
    data = np.array([])
    for path in paths:
        with open(path, 'r') as f:
            dat = json.load(f)
            dat = np.array(dat)
            data = np.concatenate((data, dat))

    X = np.zeros((0, 7))
    Y = np.zeros((0, 2))
    for _data in data:
        x = np.array([
            _data["forward"][0],
            _data["forward"][1],
            _data["forward"][2],
            _data["normX"],
            _data["normZ"],
            # _data["rotY"],
            _data["isReverse"],
            _data["velocity"],
        ])

        y = np.array([
            _data["dirV"],
            _data["dirH"]
        ])
        X = np.concatenate((X, x.reshape(1, -1)))
        Y = np.concatenate((Y, y.reshape(1, -1)))
    return X, Y

class MyDataset(Dataset):
    def __init__(self, paths):
        self.data = np.array([])
        for path in paths:
            with open(path, 'r') as f:
                data = json.load(f)
                data = np.array(data)
                self.data = np.concatenate((self.data, data))
        print("self.data:", self.data.shape)

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        _data = self.data[idx]
        rotY = modulo(_data["rotY"])
        x = np.array([
            # _data["initLoc"][0],
            # _data["initLoc"][1],
            _data["forward"][0],
            _data["forward"][1],
            _data["forward"][2],
            _data["normX"],
            _data["normZ"],
            rotY,
            _data["isReverse"],
            _data["velocity"] / 1000,
        ])

        # x = x.reshape(1, -1)
        y = np.array([
            # self.data[idx]["dirV"],
            # self.data[idx]["rotY"]
            self.data[idx]["dirH"]
        ])
        
        # y = module(y)
        # y = y.reshape(1, -1)

        return x, y

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
    
class Model(Module):
    def __init__(self, input_size=8):
        super(Model, self).__init__()

        self.fc1 = Linear(input_size, 64)
        self.fc2 = Linear(64, 128)
        self.fc3 = Linear(64, 512)
        self.fc4 = Linear(256, 128)
        self.max_pool = MaxPool1d(2)
        self.bn256 = BatchNorm1d(256)
        self.bn = BatchNorm1d(64)
        self.fc5 = Linear(64, 1)
        self.relu = PReLU()
        self.softmax = Softmax(2)
        self.flatten = Flatten()

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        x = self.relu(x)
        x = self.max_pool(x)
        x = self.bn(x)
        x = self.fc3(x)
        x = self.relu(x)
        x = self.max_pool(x)
        x = self.bn256(x)
        x = self.fc4(x)
        x = self.relu(x)
        x = self.max_pool(x)
        x = self.bn(x)
        x = self.fc5(x)
        return x

def train():
    PATH = ["..\\App\\train_data.txt", "..\\App\\train_data_1.txt", 
            "..\\App\\train_data_2.txt", "..\\App\\train_data_3.txt",
            "..\\App\\train_data_4.txt", "..\\App\\train_data_5.txt"]
    # X, y = getData(PATH)
    # y = y[:, 1]

    # DIV = 10000
    # X_train = X[:DIV, :]
    # y_train = y[:DIV]
    # X_test = X[DIV:, :]
    # y_test = y[DIV:]
    # print(X_train.shape, y_train.shape, X_test.shape, y_test.shape)
    # model = svm.SVC(kernel='linear', max_iter=1000)
    # model.fit(X_train, y_train)
    # print(model.score(X_test, y_test))
    # exit()

    dataset = MyDataset(PATH)
    train_loader = DataLoader(dataset, batch_size=BATCH, shuffle=True, drop_last=True)
    # test_loader = DataLoader(test_data, batch_size=64, shuffle=True)

    model = Model(8)
    model.to(DEVICE)

    # loss = CrossEntropyLoss()
    loss = MSELoss()
    opt = optim.SGD(model.parameters(), lr=LR, momentum=0.9)
    
    for epoch in range(EPOCH):
        losses = AverageMeter()

        idx = 0
        for x, y in tqdm(train_loader):
            idx += 1
            if idx < 20:
                continue
            # print("shape:", x.shape, y.shape)
            x = x.to(DEVICE).float()
            y = y.to(DEVICE).float()
            # print("x:", x)
            # y = modulo(y)
            pred = model(x)
            loss_out = loss(pred, y)

            losses.update(loss_out.item(), x.size(0))

            opt.zero_grad()
            loss_out.backward()
            opt.step()

        epoch_loss = losses.avg
        print(f"{epoch}. loss avg: {epoch_loss:.2f}")

if __name__ == "__main__":
    train()