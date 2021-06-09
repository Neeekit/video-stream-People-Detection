# video-stream-People-Detection

# Requirements
[CUDA](https://developer.nvidia.com/cuda-downloads)

[cuDNN](https://developer.nvidia.com/cudnn)

[OpenCV](https://opencv.org/)

[Darknet](https://github.com/AlexeyAB/darknet)

# Installation

Compile darknet with flags: `GPU=1` `OPENCV=1` `CUDNN=1` `LIBSO=1`.

In [`CMakeLists.txt`](https://github.com/Neeekit/video-stream-People-Detection/blob/34181134299d4474601e68958d432133dd836938/CMakeLists.txt#L7) set path to Darknet project folder.

From project root run `bash build.sh`.

# Using

Run `./build/PeopleDetector <path to video file>`.
