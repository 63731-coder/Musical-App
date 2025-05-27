//
// Created by Nicoleta on 25-04-25.
//

#include "AudioParam.h"
#include <mutex>


SynthParameters LockedSynthParameters::getCopy() const {
    std::lock_guard<std::mutex> lock(mutex);
    return data;
}
void LockedSynthParameters::setCopy(const SynthParameters& newData) {
    std::lock_guard<std::mutex> lock(mutex);
    data = newData;
}