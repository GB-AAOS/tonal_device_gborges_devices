#include "ProximitySubHal.h"

Return<Result> ProximitySubHal::initialize(const sp<IHalProxyCallback>&) {
    return Result::OK;
}

Return<void> ProximitySubHal::getSensorsList_2_1(getSensorsList_2_1_cb callback) {
    callback({});
    return Void();
}

Return<Result> ProximitySubHal::activate(int32_t, bool) { return Result::OK; }
Return<Result> ProximitySubHal::setOperationMode(OperationMode) { return Result::OK; }
Return<Result> ProximitySubHal::batch(int32_t, int64_t, int64_t) { return Result::OK; }
Return<Result> ProximitySubHal::flush(int32_t) { return Result::OK; }
Return<Result> ProximitySubHal::injectSensorData_2_1(const Event&) { return Result::INVALID_OPERATION; }
Return<void> ProximitySubHal::debug(const hidl_handle&, const hidl_vec<hidl_string>&) { return Void(); }