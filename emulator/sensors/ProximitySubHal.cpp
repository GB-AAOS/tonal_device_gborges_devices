#include "ProximitySubHal.h"
#include <cmath>
#include <chrono>
#include <utils/SystemClock.h>

ProximitySubHal::ProximitySubHal() {
    mSensorInfo.sensorHandle = kSensorHandle;
    mSensorInfo.name = "Proximity Fake Sensor";
    mSensorInfo.vendor = "GBorges";
    mSensorInfo.version = 1;
    mSensorInfo.type = sensors::V2_1::SensorType::PROXIMITY;
    mSensorInfo.typeAsString = "";
    mSensorInfo.maxRange = 1.0f;
    mSensorInfo.resolution = 1.0f;
    mSensorInfo.power = 0.05f;
    mSensorInfo.minDelay = 500000;   // 500ms
    mSensorInfo.fifoReservedEventCount = 0;
    mSensorInfo.fifoMaxEventCount = 0;
    mSensorInfo.requiredPermission = "";
    mSensorInfo.flags = (uint32_t)sensors::V1_0::SensorFlagBits::CONTINUOUS_MODE;
}

ProximitySubHal::~ProximitySubHal() {
    // Stop thread
    mStopThread = true;
    
    // Wait for the thread to finish execution
    if (mThread.joinable()) {
        mThread.join();
    }
}

Return<void> ProximitySubHal::getSensorsList_2_1(getSensorsList_2_1_cb callback) {
    std::vector<SensorInfo> sensors;
    sensors.push_back(mSensorInfo);
    callback(sensors);
    return Void();
}

Return<Result> ProximitySubHal::initialize(const sp<IHalProxyCallback>& halProxyCallback) {
    mCallback = halProxyCallback;
    mStopThread = false;

    mThread = std::thread(&ProximitySubHal::sensorThreadLoop, this);
    return Result::OK;
}

Return<Result> ProximitySubHal::activate(int32_t sensorHandle, bool enabled) {
    if (sensorHandle != kSensorHandle) return Result::BAD_VALUE;
    mEnabled = enabled;
    return Result::OK;
}

void ProximitySubHal::sensorThreadLoop() {
    float t = 0.0f; // Manual time accumulator
    const float kPi = 3.14159265358979323846f;

    while (!mStopThread) {
        if (mEnabled) {
            float sineValue = 0.5f + 0.5f * std::sin(t);

            // Prepare the HIDL Event
            Event event;
            event.timestamp = android::elapsedRealtimeNano();
            event.sensorHandle = kSensorHandle;
            event.sensorType = sensors::V2_1::SensorType::PROXIMITY;
            event.u.scalar = sineValue;

            std::vector<Event> events;
            events.push_back(event);

            if (mCallback != nullptr) {
                mCallback->postEvents(events, mCallback->createScopedWakelock(false));
            }

            t += 0.1f;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


// ========================================================
//                Unused methods (stubs)
// ========================================================

Return<Result> ProximitySubHal::setOperationMode(OperationMode) { return Result::OK; }
Return<Result> ProximitySubHal::batch(int32_t, int64_t, int64_t) { return Result::OK; }
Return<Result> ProximitySubHal::flush(int32_t) { return Result::OK; }
Return<Result> ProximitySubHal::injectSensorData_2_1(const Event&) { return Result::INVALID_OPERATION; }
Return<void> ProximitySubHal::debug(const hidl_handle&, const hidl_vec<hidl_string>&) { return Void(); }

// ========================================================
//                Unsported methods (stubs)
// ========================================================
 
android::hardware::Return<void> ProximitySubHal::registerDirectChannel(
    const android::hardware::sensors::V1_0::SharedMemInfo& /* mem */,
    registerDirectChannel_cb callback) {
    // We don't support direct channels for this fake sensor
    callback(android::hardware::sensors::V1_0::Result::INVALID_OPERATION, -1 /* channelHandle */);
    return android::hardware::Void();
}

android::hardware::Return<android::hardware::sensors::V1_0::Result> ProximitySubHal::unregisterDirectChannel(
    int32_t /* channelHandle */) {
    return android::hardware::sensors::V1_0::Result::INVALID_OPERATION;
}

android::hardware::Return<void> ProximitySubHal::configDirectReport(
    int32_t /* sensorHandle */, int32_t /* channelHandle */,
    android::hardware::sensors::V1_0::RateLevel /* rate */,
    configDirectReport_cb callback) {
    callback(android::hardware::sensors::V1_0::Result::INVALID_OPERATION, -1 /* sensorReportToken */);
    return android::hardware::Void();
}