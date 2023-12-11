#pragma once
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

namespace impl
{
    template<typename T>
    double variance(const std::vector<T>& vec)
    {
        const size_t size = vec.size();
        if(size <= 1) {
            return 0.0;
        }

        const double mean = std::accumulate(vec.begin(), vec.end(), T(0)) / static_cast<double>(size);

        double variance{0.0};
        for(const auto value : vec) {
            variance += (value - mean) * (value - mean);
        }
        variance /= size;
        return variance;
    }
}// namespace impl

class TimeKeeper {
public:
    void Start()
    {
        t_start_ = std::chrono::high_resolution_clock::now();
    }
    int Stop()
    {
        if(!enabled_) {
            return 0;
        }
        auto t_end = std::chrono::high_resolution_clock::now();
        int dt = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start_).count();
        measurements_.push_back(dt);
        return dt;
    }
    void Reset()
    {
        measurements_.clear();
    }
    bool Write(std::string basename)
    {
        std::ofstream file{basename + ".csv"};
        if(!file) {
            return false;
        }
        for(const auto value : measurements_) {
            file << value << "\n";
        }
        return true;
    }
    size_t Count() { return measurements_.size(); }
    void Enable() { enabled_ = true; }
    void Disable() { enabled_ = false; }

private:
    std::vector<int> measurements_;
    std::chrono::high_resolution_clock::time_point t_start_;
    bool enabled_{true};
};
