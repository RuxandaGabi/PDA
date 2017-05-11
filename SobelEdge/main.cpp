#include <iostream>
#include <opencv2/opencv.hpp>
#include <cuda.h>

using namespace cv;
using namespace std;

texture <float,2,cudaReadModeElementType> tex1;

static cudaArray *cuArray = NULL;

__global__ void sobel_x(float* output,int width,int height,int widthStep){
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if(x<width && y<height){
        float output_value = (-1*tex2D(tex1,x-1,y-1)) + (0*tex2D(tex1,x,y-1)) + (1*tex2D(tex1,x+1,y-1))
                           + (-2*tex2D(tex1,x-1,y))   + (0*tex2D(tex1,x,y))   + (2*tex2D(tex1,x+1,y))
                           + (-1*tex2D(tex1,x-1,y+1)) + (0*tex2D(tex1,x,y+1)) + (1*tex2D(tex1,x+1,y+1));

        output[y*widthStep+x]=output_value;
    }
}

void kernelcall(float* input,float* output,int width,int height,int widthStep){
    cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<float>();

    cudaMemcpy2DToArray(cuArray,0,0,input,widthStep,width * sizeof(float),height,cudaMemcpyHostToDevice);

    cudaBindTextureToArray(tex1,cuArray,channelDesc);

    float * D_output_x;

    dim3 blocksize(16,16);
    dim3 gridsize;
    gridsize.x=(width+blocksize.x-1)/blocksize.x;
    gridsize.y=(height+blocksize.y-1)/blocksize.y;

    sobel_x<<<gridsize,blocksize>>>(D_output_x,width,height,widthStep/sizeof(float));

    cudaThreadSynchronize();

    cudaUnbindTexture(tex1);
    cudaFree(D_output_x);
    cudaFreeArray(cuArray);
}

int main(int argc, char** argv){
    IplImage* image;

    image = cvLoadImage("img.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    if(!image){
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    IplImage* image2 = cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,image->nChannels);
    IplImage* image3 = cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,image->nChannels);

    cvConvert(image,image3);

    float *output = (float*)image2->imageData;
    float *input  = (float*)image3->imageData;

    kernelcall(input, output, image->width,image->height, image3->widthStep);

    cvScale(image2,image2,1.0/255.0);

    cvShowImage("Original Image", image);
    cvShowImage("Filtered Image", image2);
    cvWaitKey(0);
    return 0;
}
