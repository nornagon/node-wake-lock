#include <node_api.h>
#include <assert.h>

#include <IOKit/pwr_mgt/IOPMLib.h>

napi_value CreatePMAssertion(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);
  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments");
    return nullptr;
  }

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);
  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  if (valuetype0 != napi_string || valuetype1 != napi_boolean) {
    napi_throw_type_error(env, nullptr, "Wrong arguments");
    return nullptr;
  }

  char reason[128];
  status = napi_get_value_string_utf8(env, args[0], reason, sizeof(reason), nullptr);
  assert(status == napi_ok);

  bool shouldPreventDisplaySleep = false;
  status = napi_get_value_bool(env, args[1], &shouldPreventDisplaySleep);
  assert(status == napi_ok);

  CFStringRef assertionType = shouldPreventDisplaySleep
      ? kIOPMAssertPreventUserIdleDisplaySleep
      : kIOPMAssertPreventUserIdleSystemSleep;

  CFStringRef reasonForActivity(CFStringCreateWithCString(
      kCFAllocatorDefault, reason, kCFStringEncodingUTF8));

  IOPMAssertionID assertionId;
  IOReturn success = IOPMAssertionCreateWithName(
      assertionType,
      kIOPMAssertionLevelOn,
      reasonForActivity,
      &assertionId
  );

  CFRelease(reasonForActivity);

  if (success != kIOReturnSuccess) {
    napi_throw_error(env, nullptr, "Could not create IOPMAssertion");
    return nullptr;
  }

  napi_value ret;
  status = napi_create_uint32(env, static_cast<uint32_t>(assertionId), &ret);
  assert(status == napi_ok);
  return ret;
}

napi_value ReleasePMAssertion(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);
  if (argc < 1) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments");
    return nullptr;
  }

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  if (valuetype0 != napi_number) {
    napi_throw_type_error(env, nullptr, "Wrong arguments");
    return nullptr;
  }

  IOPMAssertionID assertionId;
  status = napi_get_value_uint32(env, args[0], static_cast<uint32_t*>(&assertionId));
  assert(status == napi_ok);

  IOReturn success;
  success = IOPMAssertionRelease(assertionId);

  if (success != kIOReturnSuccess) {
    napi_throw_error(env, nullptr, "Could not release IOPMAssertion");
    return nullptr;
  }

  return nullptr;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc[] = {
    DECLARE_NAPI_METHOD("createPMAssertion", CreatePMAssertion),
    DECLARE_NAPI_METHOD("releasePMAssertion", ReleasePMAssertion),
  };
  status = napi_define_properties(env, exports, 2, desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
