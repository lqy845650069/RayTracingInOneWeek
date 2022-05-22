// RayTracingInOneWeek.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<opencv.hpp>
#include<highgui.hpp>
#include"Vec3.h"
#include"Ray.h"
#include"Hitable.h"
#include"HitableList.h"
#include"Camera.h"
#include"Sphere.h"
using namespace cv;

HitableList random_scene() {
    HitableList world;

    world.add(make_shared<Sphere>(
        Vec3(0, -1000, 0), 1000, make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - Vec3(4, 0.2, 0)).Vec3Length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random() * random();
                    world.add(
                        make_shared<MovingSphere>(center, center + Vec3(0, random_double(0, 0.5), 0), 0.0, 1.0, 0.2, make_shared<Lambertian>(albedo)));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random(0.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(
                        make_shared<Sphere>(center, 0.2, make_shared<Metal>(albedo, fuzz)));
                }
                else {
                    // glass
                    world.add(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));

    world.add(
        make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));

    world.add(
        make_shared<Sphere>(Vec3(4, 1, 0), 1.0, make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

    return world;
}


Vec3 Color(const Ray& R, const Hitable& World, int Depth) {
    HitRecord Rec;
    if (World.Hit(R, 0.001, FLT_MAX, Rec)) {
        Ray Scattered;
        Vec3 Attenuation;
        if (Depth < 50 && Rec.MatPtr->Scatter(R, Rec, Attenuation, Scattered)) {
            return Attenuation * Color(Scattered, World, Depth + 1);
        }
        else return Vec3(0, 0, 0);
    }
    else {
        Vec3 UnitDirection = R.Direction();
        UnitDirection.MakeUnitVector();
        float T = 0.5 * (UnitDirection.Y() + 1.0);
        return (1.0 - T) * Vec3(1.0, 1.0, 1.0) + T * Vec3(0.5, 0.7, 1.0);
    }
}

int main()
{
    int imageWidth = 2000; //列
    int imageHeight = 1000; //行
    int ns = 100; //扫描次数
    auto World = random_scene();
    //World.add(make_shared<Sphere>(Vec3(0, 0, -1), 0.5, make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5))));
    //World.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100, make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0))));
    //World.add(make_shared<Sphere>(Vec3(1, 0, -1), 0.5, make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.3)));
    //World.add(make_shared<Sphere>(Vec3(-1, 0, -1), 0.5, make_shared<Dielectric>(1.5)));
    //World.add(make_shared<Sphere>(Vec3(-1, 0, -1), -0.45, make_shared<Dielectric>(1.5)));
    const auto aspectRatio = float(imageWidth) / imageHeight;
    Vec3 lookFrom(13, 2, 3);
    Vec3 lookAt(0, 0, 0);
    Vec3 up(0, 1, 0);
    auto distToFocus = 10.0;
    auto Aperture = 0.1;
    Camera Cam(lookFrom, lookAt, up, 20, aspectRatio, Aperture, distToFocus, 0.0, 1.0);

    std::cout << "P3" << std::endl << imageWidth << " " << imageHeight << std::endl << "255" << std::endl;
    
    //预览窗口
    int WindowWidth;
    int WindowHeight;

    if (imageWidth > imageHeight) {
        WindowHeight = 1080 - 200;
        WindowWidth = (int)((float)WindowHeight * (float)imageWidth / (float)imageHeight);
    }
    else {
        WindowWidth = 1920 - 200;
        WindowHeight = (int)((float)WindowWidth * (float)imageHeight / (float)imageWidth);
    }

    Mat RenderingImage(imageHeight, imageWidth, CV_8UC3, Scalar(50, 50, 50));
    namedWindow("1", WINDOW_NORMAL); //设置标题
    moveWindow("1", (int)((1920.0 - WindowWidth) / 2), (int)((1080.0 - WindowHeight) / 2) - 50);
    resizeWindow("1", WindowWidth, WindowHeight);


    for (int i = imageHeight - 1; i >= 0; --i) {
        std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
        for (int j = 0; j < imageWidth; ++j) {
            Vec3 Col(0, 0, 0);
            for (int s = 0; s < ns; ++s) {
                float U = ((float(j) + random_double(0.0, 1.0)) / float(imageWidth));
                float V = ((float(i) + random_double(0.0, 1.0)) / float(imageHeight));
                Ray R = Cam.GetRay(U, V);
                Col += Color(R, World, 0);
            }
            Col /= float(ns);
            Col = Vec3(sqrt(Col[0]), sqrt(Col[1]), sqrt(Col[2]));
            int ir = int(255.99 * Col[0]);
            int ig = int(255.99 * Col[1]);
            int ib = int(255.99 * Col[2]);

            std::cout << ir << " " << ig << " " << ib << std::endl;

            RenderingImage.at<cv::Vec3b>(imageHeight - 1 - i, j)[0] = ib;
            RenderingImage.at<cv::Vec3b>(imageHeight - 1 - i, j)[1] = ig;
            RenderingImage.at<cv::Vec3b>(imageHeight - 1 - i, j)[2] = ir;
        }
        if (!(i % (imageHeight / 100))) {
            imshow("1", RenderingImage);
            waitKey(1);
        }
    }
    imshow("1", RenderingImage);
    waitKey(3000);
    destroyAllWindows();
    return 0;
}
