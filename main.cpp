#include <stdio.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Drawing shapes 
#include <opencv2/imgproc.hpp> 
  
#include <opencv2/highgui/highgui.hpp>

void show_image(const cv::Mat& image, const std::string& winTitle);
cv::Mat loadImageAndGrayScale(const std::string& filename);
void sobelTest(const cv::Mat& grayImage);

int main(int, char**){
    cv::Mat image, grayImage;

    image, grayImage = loadImageAndGrayScale("C:/CRVW/res/vehicle_test.png");

    sobelTest(grayImage);

}


cv::Mat loadImageAndGrayScale(const std::string& filename)
{
    cv::Mat image, grayImage;
    image = cv::imread(filename);

    // Get the grayscale image
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    if(!image.data)
    {
        std::cout << "[Error] Loading image failed." << std::endl;
    }

    return image, grayImage;
}

void show_image(const cv::Mat& image, const std::string& winTitle)
{
    cv::namedWindow(winTitle, cv::WINDOW_AUTOSIZE);
    cv::imshow(winTitle, image);
    cv::waitKey();
}

void sobelTest(const cv::Mat& grayImage)
{
    cv::Mat outputImage;

    int dx = 1;
    int dy = 1;
    int sobelKernelSize = 3;
    int scaleFactor = 1;
    int deltaValue = 1;

    while (true){


        cv::Sobel(grayImage, outputImage, CV_8UC1, dx, dy, sobelKernelSize, scaleFactor, deltaValue);


        int c = cv::waitKey(1);
        // Press ESC to exit 
        if (c == 27){
            break;
        }
        else if((char)c == 'a') {
            std::cout << "pressed to a" << std::endl;
            if(dx && dy)
                dy = 0;
            else
                dy = 1;
        }
        else if((char)c == 's') {
            std::cout << "pressed to s" << std::endl;
            if(dx && dy)
                dx = 0;
            else
                dx = 1;
        }
        else if((char)c == 'd') {
            std::cout << "pressed to d" << std::endl;
            sobelKernelSize += 2;
        }
        else if((char)c == 'f') {
            std::cout << "pressed to f" << std::endl;
            if(sobelKernelSize > 1)
                sobelKernelSize -= 2;
        }
        else if((char)c == 'z') {
            std::cout << "pressed to z" << std::endl;
            scaleFactor++;
        }
        else if((char)c == 'x') {
            std::cout << "pressed to x" << std::endl;
            scaleFactor--;
        }
        else if((char)c == 'c') {
            std::cout << "pressed to c" << std::endl;
            deltaValue++;
        }
        else if((char)c == 'v') {
            std::cout << "pressed to v" << std::endl;
            deltaValue--;
        }

        cv::namedWindow("Gray Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Gray Image", grayImage);

        cv::namedWindow("Output Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Output Image", outputImage);

    }
}