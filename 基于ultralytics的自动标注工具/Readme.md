# AutoLabelme-Lin

## 简介
基于ultralytics的数据集自动标注工具，格式为labelme的json格式

## 使用方法
在选定模型目录，图片/视频目录后，点击**开始识别**即可开始自动标注
<img src="images/mainwindow.png" style="zoom:50%;" />

其中，图片类会生成同名的json文件，视频类会生成同名目录，在目录下保存识别成功的帧，并生成同名的json文件，命名为按顺序的8位数字

可以选择**一键导入**选择父目录实现图像/视频/模型的一次性导入，该目录的子目录命名无特殊要求，因为会遍历该目录下的所有文件，根据文件后缀名判断属于哪种类型
<img src="images/all_dir.png" style="zoom:50%;" />

## 配置方法
有`conda`环境可以直接利用`env.yaml`配置
```
conda env create -f env.yaml
```

但其中的`pytorch`为`cuda12.1`版本，需要其他版本需要另外配置
```
conda uninstall pytorch torchvision torchaudio
conda install pytorch torchvision torchaudio pytorch-cuda="cuda版本号" -c pytorch -c nvidia
```

若无`conda`环境，可以手动配置`requirements.txt`中的依赖
```
pip install -r requirements.txt
```

**但在测试过程中发现，使用`pip`安装的`pytorch`或在使用`ultralytics`进行识别时会出现大量乱识别错识别的情况，但使用conda安装则无这种情况，暂不清楚原因**


## 打包为二进制文件
开始尝试将这个工具打包为二进制工具，但使用时出现一些情况
1. `ultralytics`找不到`defaylt.yaml`文件
    这个主要是因为`ultralytics`会默认寻找`defaylt.yaml`文件，但打包后由于路径不同，所以找不到该文件，而其源码中也没有下载该文件的步骤，所以需要自行添加下载代码

    ``` python
    # ultralytics/util/init.py
    import requests
    if not os.path.exists(DEFAULT_CFG_PATH):
        url = 'https://raw.githubusercontent.com/ultralytics/ultralytics/main/ultralytics/cfg/default.yaml'
        # 发送HTTP请求获取文件内容
        response = requests.get(url, stream=True)
        # 检查请求是否成功
        if response.status_code == 200:
            # 确保保存路径的目录存在
            os.makedirs(os.path.dirname(DEFAULT_CFG_PATH), exist_ok=True)
            
            # 打开文件并写入内容
            with open(DEFAULT_CFG_PATH, 'wb') as file:
                for chunk in response.iter_content(chunk_size=8192):
                    file.write(chunk)

        else:
            raise f"下载失败，状态码：{response.status_code}"
    ```

2. 解决上一个问题后，发现在实际进行识别标注时，在加载模型时会自动打开一个新的程序窗口，关掉该程序窗口后，才能继续加载模型并完成识别标注，暂不清楚原因。