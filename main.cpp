#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Drawing shapes 
#include <opencv2/imgproc.hpp> 
  
#include <opencv2/highgui/highgui.hpp>

void show_image(const cv::Mat& image, const std::string& winTitle);
cv::Mat loadImageAndGrayScale(const std::string& filename);
void sobelTest(const cv::Mat& grayImage);
double calculateAverageIntensity(const cv::Mat& image, int uL, int uR, int v);
void createSetsTest(const cv::Mat& grayImage);

int main(int, char**){
    cv::Mat image, grayImage;

    image, grayImage = loadImageAndGrayScale("C:/CRVW/res/vehicle_test.png");
    // show_image(grayImage,"Original");
    
    createSetsTest(grayImage);

        // // Step 2: Match pixels from NE with pixels from PE
        // std::vector<std::pair<int, int>> VSS_segments;  // Store valid VSS segments as pairs of (ux, uy)

        // // Step 4: Repeat Step 2 and Step 3 until NE is empty
        // while (!NE.empty()) {
        //     bool foundSegment = false;

        //     for (int i = 0; i < NE.size(); ++i) {
        //         int ux = NE[i];

        //         for (int uy : PE) {
        //             // Check if the segment satisfies the width constraint
        //             int width = uy - ux;
        //             double intent = calculateAverageIntensity(grayImage, ux, uy, v);
        //             if (width >= LL && width <= LH && intent < PT) {
        //                 // Valid VSS segment found
        //                 VSS_segments.push_back(std::make_pair(ux, uy));
        //                 foundSegment = true;
        //                 // Step 3: Remove all pixels in NE that satisfy u ≤ uy
        //                 NE.erase(std::remove_if(NE.begin(), NE.end(), [uy](int u) { return u <= uy; }), NE.end());
        //                 break;  // Move to the next NE pixel after finding the first valid PE match
        //             }
        //         }

        //         if (foundSegment) {
        //             break;  // Move to the next iteration of the while loop
        //         }
        //     }

        //     if (!foundSegment) {
        //         // If no segment is found, exit the loop to avoid infinite loops
        //         break;
        //     }
        // }

        // Mark the valid VSS segments in green on the color image
        // for (const auto& segment : VSS_segments) {
        //     int ux = segment.first;
        //     int uy = segment.second;

        //     // Draw the VSS segment as a green line
        //     cv::line(colorImage, cv::Point(ux, v), cv::Point(uy, v), cv::Scalar(0, 255, 0), 1);
        // }

    return 0;

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

double calculateAverageIntensity(const cv::Mat& image, int uL, int uR, int v) {
    // Ensure the image is in grayscale
    if (image.channels() != 1) {
        std::cerr << "Error: The image must be in grayscale." << std::endl;
        return -1;
    }

    // Ensure uL and uR are within the image boundaries
    uL = std::max(0, uL);
    uR = std::min(image.cols - 1, uR);

    // Initialize the sum and count
    int sum = 0;
    int count = uR - uL + 1;

    // Loop through the pixels between uL and uR on the horizontal line v
    for (int u = uL; u <= uR; ++u) {
        sum += image.at<uchar>(v, u);  // Sum the intensity values
    }

    // Calculate the average intensity
    double averageIntensity = static_cast<double>(sum) / count;

    return averageIntensity;
}

void createSetsTest(const cv::Mat& grayImage)
{
    cv::Mat sobelX;

    int dx = 1;
    int dy = 1;
    int sobelKernelSize = 3;
    int scaleFactor = 1;
    int deltaValue = 1;

    int height = grayImage.rows;
    int width = grayImage.cols;

    int PT = 100;
    int ST = 100;

    int LL = 25;
    int LH = 150;

    while (true){

        cv::Mat colorImage;
        cv::cvtColor(grayImage, colorImage, cv::COLOR_GRAY2BGR);
        cv::Sobel(grayImage, sobelX, CV_64F, dx, dy, sobelKernelSize, scaleFactor, deltaValue);

        for (int v = 0; v < height; ++v){
            std::vector<int> NE;
            std::vector<int> PE;
            
            for (int i = 0; i < width; ++i){
                int pixelIntensity = grayImage.at<uchar>(v, i);
                int sobelValue = sobelX.at<double>(v,i);

                if (pixelIntensity < PT && sobelValue < -ST) {
                    // Add pixel to NE set
                    NE.push_back(i);
                }
                else if (pixelIntensity < PT && sobelValue > ST) {
                    // Add pixel to PE set
                    PE.push_back(i);
                }
            }

            // Mark NE pixels in red and PE pixels in blue on the color image
            for (int u : NE) {
                colorImage.at<cv::Vec3b>(v, u) = cv::Vec3b(0, 0, 255);  // Red color (BGR format)
            }
            for (int u : PE) {
                colorImage.at<cv::Vec3b>(v, u) = cv::Vec3b(255, 0, 0);  // Blue color (BGR format)
            }


            // // Step 2: Match pixels from NE with pixels from PE
            std::vector<std::pair<int, int>> VSS_segments;  // Store valid VSS segments as pairs of (ux, uy)

            // Step 4: Repeat Step 2 and Step 3 until NE is empty
            while (!NE.empty()) {
                bool foundSegment = false;

                for (int i = 0; i < NE.size(); ++i) {
                    int ux = NE[i];

                    for (int uy : PE) {
                        // Check if the segment satisfies the width constraint
                        int width = uy - ux;
                        double intent = calculateAverageIntensity(grayImage, ux, uy, v);
                        if (width >= LL && width <= LH && intent < PT) {
                            // Valid VSS segment found
                            VSS_segments.push_back(std::make_pair(ux, uy));
                            foundSegment = true;
                            // Step 3: Remove all pixels in NE that satisfy u ≤ uy
                            NE.erase(std::remove_if(NE.begin(), NE.end(), [uy](int u) { return u <= uy; }), NE.end());
                            break;  // Move to the next NE pixel after finding the first valid PE match
                        }
                    }

                    if (foundSegment) {
                        break;  // Move to the next iteration of the while loop
                    }
                }

                if (!foundSegment) {
                    // If no segment is found, exit the loop to avoid infinite loops
                    break;
                }
            }

            // Mark the valid VSS segments in green on the color image
            for (const auto& segment : VSS_segments) {
                int ux = segment.first+1;
                int uy = segment.second-1;

                // Draw the VSS segment as a green line
                cv::line(colorImage, cv::Point(ux, v), cv::Point(uy, v), cv::Scalar(0, 255, 0), 1);
            }

        }

        cv::namedWindow("Gray Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Gray Image", grayImage);

        // Display the resulting image
        cv::namedWindow("VSS Extraction", cv::WINDOW_AUTOSIZE);
        cv::imshow("VSS Extraction", colorImage);


        int c = cv::waitKey(1);
        // Press ESC to exit 
        if (c == 27){
            break;
        }
        else if((char)c == 'a') {
            std::cout << "pressed to a" << std::endl;
            std::cout << "Vertical Edge Detection" << std::endl;
            if(dx && dy)
                dy = 0;
            else
                dy = 1;
            std::cout << "dx -> " << dx << ", dy -> " << dy << "\n";
        }
        else if((char)c == 's') {
            std::cout << "pressed to s" << std::endl;
            std::cout << "Horizontal Edge Detection" << std::endl;
            if(dx && dy)
                dx = 0;
            else
                dx = 1;
            std::cout << "dx -> " << dx << ", dy -> " << dy << "\n";
        }
        else if((char)c == 'd') {
            std::cout << "pressed to d" << std::endl;
            sobelKernelSize += 2;
            std::cout << "sobelKernelSize: " << sobelKernelSize << std::endl;
        }
        else if((char)c == 'f') {
            std::cout << "pressed to f" << std::endl;
            if(sobelKernelSize > 1)
                sobelKernelSize -= 2;
            std::cout << "sobelKernelSize: " << sobelKernelSize << std::endl;
        }
        else if((char)c == 'z') {
            std::cout << "pressed to z" << std::endl;
            scaleFactor++;
            std::cout << "scaleFactor: " << scaleFactor << std::endl;
        }
        else if((char)c == 'x') {
            std::cout << "pressed to x" << std::endl;
            scaleFactor--;
            std::cout << "scaleFactor: " << scaleFactor << std::endl;
        }
        else if((char)c == 'c') {
            std::cout << "pressed to c" << std::endl;
            deltaValue++;
            std::cout << "deltaValue: " << deltaValue << std::endl;
        }
        else if((char)c == 'v') {
            std::cout << "pressed to v" << std::endl;
            deltaValue--;
            std::cout << "deltaValue: " << deltaValue << std::endl;
        }
        else if((char)c == 'h') {
            std::cout << "pressed to h" << std::endl;
            PT += 10;
            std::cout << "PT: " << PT << std::endl;
        }
        else if((char)c == 'j') {
            std::cout << "pressed to j" << std::endl;
            PT -= 10;
            std::cout << "PT: " << PT << std::endl;
        }
        else if((char)c == 'k') {
            std::cout << "pressed to k" << std::endl;
            ST += 10;
            std::cout << "ST: " << ST << std::endl;
        }
        else if((char)c == 'l') {
            std::cout << "pressed to l" << std::endl;
            ST -= 10;
            std::cout << "ST: " << ST << std::endl;
        }

    }
}