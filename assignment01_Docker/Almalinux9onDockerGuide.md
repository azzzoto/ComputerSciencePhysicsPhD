# **How to Install Docker on Windows with WSL 2 and Run an AlmaLinux 9 Container**
---

# **1. Introduction**  
[Docker](https://www.docker.com/) is a powerful tool that allows you to **run applications inside containers**, which are lightweight, portable environments that include everything an application needs to work. This makes software installation and deployment much easier and more reliable.  

Since Docker requires **a Linux-based system** to function properly, installing it on Windows requires setting up a **Linux-compatible environment**. There are two ways to achieve this:
1. **Hyper-V:** A built-in virtualization technology in Windows that runs a full Linux virtual machine (VM) to host Docker.  
2. **WSL 2:** A lightweight Linux compatibility layer that allows running Linux distributions natively inside Windows, with full access to the Windows filesystem and better resource efficiency. 

While both methods work, this guide focuses on **WSL 2** because it offers better compatibility and flexibility for most users.

## **1.1 Why Choose WSL 2 Over Hyper-V?**  
While **Hyper-V is a great option**, we use **WSL 2** in this guide because:  
* **Better Performance**: WSL 2 runs with a lightweight virtual machine that is faster and uses fewer resources compared to Hyper-V.  
* **More Compatibility**: WSL 2 allows you to use Docker alongside other virtualization tools like VirtualBox, which Hyper-V often conflicts with.  
* **Easier File Access**: WSL 2 lets you seamlessly share files between Windows and Linux, making development smoother.  
* **No Need for Windows Pro**: Hyper-V requires **Windows 10/11 Pro**, while WSL 2 works on **Windows 10/11 Home** too.  

Additionally, beyond Docker, WSL 2 is particularly useful for developers who need Linux commands and tools on Windows without setting up a full virtual machine.

---

# **2. Check System Requirements**
Before installing Docker, make sure your computer meets the requirements:

* **Windows 10 (build 19045 or later)** or **Windows 11**.
* **64-bit processor**.
* **At least 4GB of RAM (8GB+ recommended)**.
* **Admin access to install software**.
* **Enabled hardware virtualization in BIOS.** Note that many Windows devices already have virtualization enabled, so this may not apply but for more informations please refer to the [Docker's official documentation on this topic](https://docs.docker.com/desktop/troubleshoot-and-support/troubleshoot/topics/#virtualization).

---

## **3. Install Windows Subsystem for Linux (WSL)**
This Docker Desktop installation requires **WSL 2**, but since it's not always installed by default, in this guide we will show how to install it.

### **Step 1: Open PowerShell as Administrator**
1. Press `Win + S` and search for **PowerShell** or **Terminal**. At this point right-click with the mouse and select *run as Administrator* ("Esegui come amministratore" in italian).  
![image](./images/powershell-wsl.png)
2. Run this command to install WSL:
   ```powershell
   wsl --install
   ```
   This installs WSL with **Ubuntu** as the default Linux distribution. To change this please refer to the official Microsoft Guide on WSL installation at the paragraph [*changing the default Linux distribution installed*.](https://learn.microsoft.com/en-us/windows/wsl/install#change-the-default-linux-distribution-installed)

### **Step 2: Set WSL Version to 2**
After installation, make sure WSL is using version **2**:
```powershell
wsl --set-default-version 2
```

### **Step 3: Restart Your Computer**
After installing WSL, restart your PC to apply changes.

---

## **4. Install Docker Desktop on Windows**
Now that WSL is set up, let's install Docker.

### **Step 1: Download Docker Desktop**
1. Go to [Docker's official website](https://www.docker.com/products/docker-desktop/).
2. Click **Download for Windows**.
![image](./images/docker-desktop-download.png)
3. Run the **Docker Desktop Installer.exe** file.

### **Step 2: Install Docker**
1. When prompted, select **“Use WSL 2 instead of Hyper-V”** (this is important). ![image](./images/wsl2_over_hyperv.png)
2. Click **Install** and wait for the process to finish.
3. Once done, click on **close and restart** to reboot.

### **Step 3: Start Docker**
1. Open **Docker Desktop** from the Start menu.
2. Accept the license agreement.
3. If asked, **enable WSL integration**.

### **Step 4: Verify Docker Installation**
To check if Docker is installed correctly, open **PowerShell** or **Windows Terminal** and type:
```powershell
docker --version
```
If it returns something like:
```
Docker version 24.x.x, build xxxxxx
```
Docker is installed successfully!

#### Troubleshoot
If you see the error message `'docker' is not recognized as an internal or external command, operable program or batch file` when running `docker --version`, try the following troubleshooting steps:

1. **Reboot Your System:** Sometimes a simple system restart can resolve environment path issues or finalize Docker installation.

2. **Ensure Docker Desktop is Running:** If you haven't opened Docker Desktop before running Docker commands, the Docker Engine might not be running. Open Docker Desktop and ensure it is properly started. The Docker CLI won't work if the engine isn't running.  
   ![image](./images/docker-engine-not-running.png)

3. **Verify Docker Installation Path:** Ensure that Docker's installation path is correctly added to your system's **PATH** environment variable. If the path is missing, the terminal won't recognize the `docker` command. You can add it manually by adding the Docker's path to the **PATH** variable ![image](./images/docker-path.png).

4. **Check Docker Desktop Settings:** Sometimes, Docker Desktop might not automatically start on boot or might face issues with virtualization settings. Open Docker Desktop and check for any alerts or error messages in the settings.

5. **Reinstall Docker:** If none of the above steps work, try reinstalling Docker. This can fix issues caused by incomplete installation or configuration errors.

6. **Verify WSL 2 Configuration (for WSL Users):** If you're using Docker with WSL 2, ensure that your WSL 2 installation is correctly set up and that Docker is integrated with your chosen Linux distribution. You can check by running `wsl --list --verbose` to see if WSL 2 is properly configured.

---

## **5. Run an AlmaLinux 9 Container**
Now let's download and run an **AlmaLinux 9** container! This is the step where we finally get to launch a Linux-based container on your machine, allowing us to run a full AlmaLinux environment inside Docker.

### **Step 1: Open Terminal inside Docker Desktop**
**Disclaimer**: We could have used **PowerShell** or **Windows Terminal** to run the following commands, but we will use the **Terminal** inside Docker Desktop to make it easier to understand since it's the first time we open it.

1. **Open Docker Desktop**: Make sure Docker Desktop is running. You should see the Docker icon in your system tray (bottom-right corner of your screen for Windows). If Docker Desktop is not open, click the icon to launch it. ![image](./images/system-tray-win.png)
2. **Enable the Docker Terminal**: In Docker Desktop, click on the terminal icon located in the top-right corner of the Docker Desktop window. This icon looks like a small computer terminal. Once you click on it, a terminal window will open, allowing you to enter commands directly.
![image](./images/docker-terminal.png)
3. Pull the AlmaLinux 9 Image: In the terminal window, type the following command to pull the AlmaLinux 9 image from Docker Hub:
   ```bash
   docker pull almalinux:9
   ```
   This will download the AlmaLinux 9 image, which is a minimal Linux-based operating system compatible with Docker. A Docker image is a lightweight, standalone, and executable package that includes everything needed to run a piece of software, including the code, runtime, libraries, and dependencies.
   
   After performing the command, you should see something like this:
   ```
   Using default tag: latest
   latest: Pulling from library/almalinux
   Digest: sha256:1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef
   Status: Downloaded newer image for almalinux:latest
   ```
   And in the Docker Desktop interface, you should see the image being downloaded in the "Images" tab.
   ![image](./images/docker-desktop-almalinux-downloading.png)

### **Step 2: Run the AlmaLinux Container**
To start a new AlmaLinux 9 container run the following command:
```powershell
docker run -it --name comp-sci-course-alma-container almalinux:9 bash
```
- `-it`: Runs it in interactive mode.
- `--name comp-sci-course-alma-container`: Names the container `comp-sci-course-alma-container`. This is useful to identify or reuse the container later.
- `almalinux:9`: Uses the AlmaLinux 9 image we've downloaded in the previous step.
- `bash`: Opens a Bash shell inside the container.

At this point you should see the container being started in the Docker Desktop interface terminal with something as `[root@2f2e3e285d47 /]#`. This means that we are inside the container!

To verify we're working inside the container, type `cat /etc/os-release` and you should see something like this:
```
NAME="AlmaLinux"
VERSION="9.x"
```
which means that we are working inside an AlmaLinux 9 container.
![image](./images/alma-container.png)

### **Step 3: Exit the Container**
To exit the AlmaLinux container, type:
```bash
exit
```

---

## **6. Basic Docker Commands**
Now that Docker is installed and you've run a container, here are some useful commands:

### **List All Containers**
```powershell
docker ps -a
```
This shows all running and stopped containers.

### **Start a Stopped Container**
```powershell
docker start comp-sci-course-alma-container
```

### **Access a Running Container**
```powershell
docker exec -it comp-sci-course-alma-container bash
```

### **Stop a Running Container**
```powershell
docker stop comp-sci-course-alma-container
```

### **Remove a Container**
```powershell
docker rm comp-sci-course-alma-container
```

### **Remove an Image**
```powershell
docker rmi almalinux:9
```
---

## **Conclusion**
You have successfully:
1. Installed WSL 2  
2. Installed Docker  
3. Downloaded and run an **AlmaLinux 9** container  

Now, you're ready to start using Docker for Linux-based applications! 🚀