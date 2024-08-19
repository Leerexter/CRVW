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

int main(int, char**){
    cv::Mat image, grayImage;

    image, grayImage = loadImageAndGrayScale("C:/Users/tleef/Desktop/FYP/res/image2.jpg");

    show_image(grayImage, "Gray Image");
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
    cv::waitKey(0);
}