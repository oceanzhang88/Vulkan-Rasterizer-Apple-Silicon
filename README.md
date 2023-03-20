# A Vulkan Rasterizer

This C++ project is to build a modern rasterization pipeline around the Vulkan graphics API. 

![vulkan_pipeline_block_diagram](https://user-images.githubusercontent.com/25319668/221376812-bfd1e65e-19df-440b-b30f-47f6c5b1b728.png)

## Command Buffer and Pipelines 

GPU Memory and Command Execution   |  A Rasterization Pipeline Data Structure  within Command
:-------------------------:|:-------------------------:
  <img width="1275" alt="Screenshot 2023-03-19 at 19 10 51" src="https://user-images.githubusercontent.com/25319668/226215876-b39afd85-4124-438d-b2b7-d9426e12eb9d.png"> |  <img width="1268" alt="Screenshot 2023-03-19 at 19 11 04" src="https://user-images.githubusercontent.com/25319668/226215875-f290278f-4f60-42c4-8f57-f49a9935314d.png">


A General Purpose Compute Pipeline Data Structure within Command   |  A Ray Tracing Pipeline Data Structure within Command (2020 and later)
:-------------------------:|:-------------------------:
<img width="851" alt="Screenshot 2023-03-19 at 19 11 25" src="https://user-images.githubusercontent.com/25319668/226215874-a0f56974-5007-476b-8ed9-01c5dbbf3b89.png"> | <img width="1189" alt="Screenshot 2023-03-19 at 19 12 51" src="https://user-images.githubusercontent.com/25319668/226215873-0ee01de6-a2f7-4e58-9d2f-8a8f4e5d8e73.png">  

## Commands and Synchronization

<img width="1254" alt="Screenshot 2023-03-19 at 19 40 30" src="https://user-images.githubusercontent.com/25319668/226217576-2bc72ef1-15e6-4cdc-8579-446b9536362a.png">

### Race Conditions

1. Read-after-Write (R-a-W) – the memory write in one operation starts overwriting the memory that another operation’s read needs to use.
2. Write-after-Read (W-a-R) – the memory read in one operation hasn’t yet finished before another operation starts overwriting that memory.
3. Write-after-Write (W-a-W) – two operations start overwriting the same memory and the end result is non-deterministic.

### Sync Locks 

- Wait Idle Operations (GPU -> CPU sync, CPU waits on queue or device)
- Fences (GPU -> CPU sync, CPU waits on a fenced subset of a queue and prior command buffers)
- Semaphores (queue sync)
  - Binary Semaphores (queue -> queue sync, GPU only)
  - Timeline Semaphores (queue -> queue sync, CPU <-> GPU sync)
- Pipeline Barriers (command -> command sync, intra-queue)
  - Execution Barriers (execution-only dependency, memory disregarded)
  - Memory Barriers (execution and memory dependencies)
- Render Pass Subpass Dependencies (subpass memory dependencies)
- Events (“split barriers”, CPU -> GPU sync)

## About

Modern Vulkan Graphics API is important in many ways. This project uses Vulkan to build a rasterization pipeline on Apple Silicon M1/2 chip. The default rasterization pipeline is Forward light architecture.

The components of this rasterizer incude: projective camera, shader loading and parsing, texture, Vulkan pipeline components: Device, Swap Chain, Command Buffer, Descriptor, Push Constant, loading obj models, multi-point Lighting, alpha blending and more.
Meanwhile, you can move the camera around using keyboard keys.

***NOTE:*** Lighting pipeline architecture choice (Forward/Deferred/Tile-based Forward/Tile-based Deferred) is how we orchestrate Vulkan API not a built-in part of Vulkan or any other Graphics APIs.

In deferred lighting architecture, we render objects to G-Buffer first and then render lights reading from G-Buffer. In forward mode, lights and objects are coupled together.

Tile-based Deferred architecture is proposed by Apple:

[TBDR](https://developer.apple.com/documentation/metal/tailor_your_apps_for_apple_gpus_and_tile-based_deferred_rendering)

Tile-based Forward architecture is proposed by a paper with a fancy name:

[Foward+](https://takahiroharada.files.wordpress.com/2015/04/forward_plus.pdf)

## Demo
 
https://user-images.githubusercontent.com/25319668/224529664-1a15a924-b302-403b-b52c-5e5201144ef7.mp4

## Requirements

C++ Lib:
* GLM
* GLFW

Vulkan SDK:
* https://vulkan.lunarg.com/sdk/home

## Build on MacOS

C++17 is needed to compile the project.
```
./macBuild
```
## Run on MacOS
```
cd build
./Vulkan-Rasterizer
```

## Keyboard Controls

* W/A/S/D/E/Q to change the camera positions.

* Arrow key up/left/right/down to rotate the camera.


## Credits

1. Thanks to the wonderful tutorial by [Brendan](https://www.youtube.com/@BrendanGalea)
2. Thanks to the [Vulkan Docs](https://vulkan.lunarg.com/doc/sdk/1.3.239.0/windows/getting_started.html)
