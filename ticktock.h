#pragma once
#include<chrono>
#define TICK(x) auto now_##x = std::chrono::steady_clock::now();
#define TOCK(x) std::cout<<#x":"<<std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now()-now_##x).count()<<"s"<<endl;