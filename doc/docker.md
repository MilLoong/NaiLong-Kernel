# Docker 开发环境配置

本文档介绍如何使用 Docker 搭建 NaiLong-Kernel 的开发环境。镜像由仓库内 `tools/Dockerfile` 构建。

## 目录

- [构建并运行](#构建并运行)
- [SSH 配置](#ssh-配置)
- [VSCode 远程开发](#vscode-远程开发)
- [常用命令](#常用命令)

## 构建并运行

```shell
cd NaiLong-Kernel

# 构建开发镜像
docker build -t nailong-kernel-dev:latest --target nailong_kernel ./tools

# 启动容器
docker run --name NaiLong-Kernel-container -itd \
  -p 233:22 \
  -v ./:/root/NaiLong-Kernel \
  -v ~/.ssh:/root/.ssh \
  -v ~/.gitconfig:/root/.gitconfig \
  nailong-kernel-dev:latest

# 进入容器
docker exec -it NaiLong-Kernel-container /bin/zsh
```

### 验证环境

```shell
gcc --version
clang --version
aarch64-linux-gnu-gcc --version
riscv64-linux-gnu-gcc --version
cmake --version
make --version

cd /root/NaiLong-Kernel
cmake --preset build_x86_64
cmake --build build_x86_64 --target NaiLong-Kernel -j"$(nproc)"
```

VS Code / Cursor 也可直接用 `.devcontainer`：打开仓库后选 **Reopen in Container**，会按同一 Dockerfile 构建。

## SSH 配置

为了更好的开发体验，可以配置 SSH 远程访问：

### 1. 生成 SSH 密钥（本地）

```shell
# 检查是否已存在 SSH 密钥
ls ~/.ssh/

# 如果不存在，生成新的 RSA 密钥
# 将 <your-email> 替换为你的邮箱
ssh-keygen -t rsa -b 4096 -C "<your-email>"

# 查看公钥内容
cat ~/.ssh/id_rsa.pub
```

### 2. 配置容器 SSH 访问

```shell
# 通过 SSH 连接到容器（首次需要密码）
ssh -p 233 zone@localhost
# 默认密码：zone

# 在容器内创建 SSH 目录和授权文件
mkdir -p /home/zone/.ssh
touch /home/zone/.ssh/authorized_keys
chmod 700 /home/zone/.ssh
chmod 600 /home/zone/.ssh/authorized_keys

# 将本地公钥内容添加到 authorized_keys
# 可以通过 docker exec 或直接编辑文件
```

### 3. 验证 SSH 免密登录

```shell
# 现在应该可以免密登录
ssh -p 233 zone@localhost
```

## VSCode 远程开发

### 1. 安装插件

在 VSCode 中安装以下插件：
- `Remote - SSH`
- `Remote - Containers` (可选)

### 2. 配置 SSH 连接

1. 打开命令面板：`Ctrl+Shift+P` (Windows/Linux) 或 `Cmd+Shift+P` (macOS)
2. 输入：`Remote-SSH: Add New SSH Host...`
3. 输入 SSH 命令：`ssh -p 233 zone@localhost`
4. 选择配置文件保存位置

### 3. 连接并打开项目

1. 打开命令面板：`Ctrl+Shift+P` / `Cmd+Shift+P`
2. 输入：`Remote-SSH: Connect to Host...`
3. 选择 `zone@localhost`
4. 在新窗口中打开文件夹：`/home/zone/NaiLong-Kernel`

### 4. 推荐插件（远程环境）

在远程环境中安装以下插件以获得更好的开发体验：
- C/C++ Extension Pack
- CMake Tools
- GitLens
- Clang-Format

## 常用命令

### 容器管理

```shell
docker ps -a
docker start NaiLong-Kernel-container
docker stop NaiLong-Kernel-container
docker restart NaiLong-Kernel-container
docker rm NaiLong-Kernel-container
docker logs NaiLong-Kernel-container
```

### 镜像管理

```shell
docker images
docker rmi nailong-kernel-dev:latest
docker image prune
```

### 文件传输

```shell
docker cp NaiLong-Kernel-container:/path/to/file /host/path/
docker cp /host/path/file NaiLong-Kernel-container:/path/to/
```

## 故障排除

1. **端口 233 已被占用**
   ```shell
   lsof -i :233
   docker run -p 234:22 ...
   ```

2. **容器启动失败**
   ```shell
   docker logs NaiLong-Kernel-container
   ```

3. **挂载目录权限问题**
   ```shell
   ls -la ./
   ```

4. **SSH 连接失败**
   ```shell
   docker ps
   docker exec -it NaiLong-Kernel-container systemctl status ssh
   ```

### 重置环境

```shell
docker stop NaiLong-Kernel-container
docker rm NaiLong-Kernel-container

docker run --name NaiLong-Kernel-container -itd \
  -p 233:22 \
  -v ./:/root/NaiLong-Kernel \
  -v ~/.ssh:/root/.ssh \
  -v ~/.gitconfig:/root/.gitconfig \
  nailong-kernel-dev:latest
```
