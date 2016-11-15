/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_C_H_
#define CSCORE_C_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct CvMat;

//
// The C API is handle-based.  Sources and sinks are reference counted
// internally to the library.  Any time a source or sink handle is returned
// or provided to a callback, the reference count is incremented.
// Calling CS_ReleaseSource() or CS_ReleaseSink() decrements the reference
// count, and when the reference count reaches zero, the object is destroyed.
// Connecting a source to a sink increments the reference count of the source,
// and when the sink is destroyed (its reference count reaches zero), the
// source reference count is decremented.
//

//
// Typedefs
//
typedef int CS_Bool;
typedef int CS_Status;

typedef int CS_Handle;
typedef CS_Handle CS_Property;
typedef CS_Handle CS_Listener;
typedef CS_Handle CS_Sink;
typedef CS_Handle CS_Source;

//
// Status values
//
enum CS_StatusValue {
  CS_PROPERTY_WRITE_FAILED = 2000,
  CS_OK = 0,
  CS_INVALID_HANDLE = -2000,    // handle was invalid (does not exist)
  CS_WRONG_HANDLE_SUBTYPE = -2001,
  CS_INVALID_PROPERTY = -2002,
  CS_WRONG_PROPERTY_TYPE = -2003,
  CS_READ_FAILED = -2004,
  CS_SOURCE_IS_DISCONNECTED = -2005
};

//
// Pixel formats
//
enum CS_PixelFormat {
  CS_PIXFMT_UNKNOWN = 0,
  CS_PIXFMT_MJPEG,
  CS_PIXFMT_YUYV,
  CS_PIXFMT_RGB565
};

//
// Frame formats
//
typedef struct CS_VideoMode {
  int pixelFormat;
  int width;
  int height;
  int fps;
} CS_VideoMode;

//
// Property types
//
enum CS_PropertyType {
  CS_PROP_NONE = 0,
  CS_PROP_BOOLEAN = 1,
  CS_PROP_INTEGER = 2,
  CS_PROP_STRING = 4,
  CS_PROP_ENUM = 8
};

//
// Listener event types
//
enum CS_EventType {
  CS_SOURCE_CREATED = 0x0001,
  CS_SOURCE_DESTROYED = 0x0002,
  CS_SOURCE_CONNECTED = 0x0004,
  CS_SOURCE_DISCONNECTED = 0x0008,
  CS_SOURCE_VIDEOMODES_UPDATED = 0x0010,
  CS_SOURCE_VIDEOMODE_CHANGED = 0x0020,
  CS_SINK_SOURCE_CHANGED = 0x0040,
  CS_SINK_CREATED = 0x0100,
  CS_SINK_DESTROYED = 0x0200,
  CS_SINK_ENABLED = 0x0400,
  CS_SINK_DISABLED = 0x0800,
  CS_SOURCE_PROPERTY_CREATED = 0x1000,
  CS_SOURCE_PROPERTY_VALUE_UPDATED = 0x2000,
  CS_SOURCE_PROPERTY_CHOICES_UPDATED = 0x4000
};

//
// Listener event
//
struct CS_Event {
  CS_EventType type;

  // Valid for CS_SOURCE_* and CS_SINK_* respectively
  CS_Source source;
  CS_Sink sink;

  // Source/sink name
  const char *name;

  // Fields for CS_SOURCE_VIDEOMODE_CHANGED event
  CS_VideoMode mode;

  // Fields for CS_SOURCE_PROPERTY_* events
  CS_Property property;
  CS_PropertyType propertyType;
  int value;
  const char* valueStr;
};

//
// Property Functions
//
enum CS_PropertyType CS_GetPropertyType(CS_Property property,
                                        CS_Status* status);
char* CS_GetPropertyName(CS_Property property, CS_Status* status);
int CS_GetProperty(CS_Property property, CS_Status* status);
void CS_SetProperty(CS_Property property, int value, CS_Status* status);
int CS_GetPropertyMin(CS_Property property, CS_Status* status);
int CS_GetPropertyMax(CS_Property property, CS_Status* status);
int CS_GetPropertyStep(CS_Property property, CS_Status* status);
int CS_GetPropertyDefault(CS_Property property, CS_Status* status);
char* CS_GetStringProperty(CS_Property property, CS_Status* status);
void CS_SetStringProperty(CS_Property property, const char* value,
                          CS_Status* status);
char** CS_GetEnumPropertyChoices(CS_Property property, int* count,
                                 CS_Status* status);

//
// Source Creation Functions
//
CS_Source CS_CreateUSBCameraDev(const char* name, int dev, CS_Status* status);
CS_Source CS_CreateUSBCameraPath(const char* name, const char* path,
                                 CS_Status* status);
CS_Source CS_CreateHTTPCamera(const char* name, const char* url,
                              CS_Status* status);
CS_Source CS_CreateCvSource(const char* name, const CS_VideoMode* mode,
                            CS_Status* status);

//
// Source Functions
//
char* CS_GetSourceName(CS_Source source, CS_Status* status);
char* CS_GetSourceDescription(CS_Source source, CS_Status* status);
uint64_t CS_GetSourceLastFrameTime(CS_Source source, CS_Status* status);
CS_Bool CS_IsSourceConnected(CS_Source source, CS_Status* status);
CS_Property CS_GetSourceProperty(CS_Source source, const char* name,
                                 CS_Status* status);
CS_Property* CS_EnumerateSourceProperties(CS_Source source, int* count,
                                          CS_Status* status);
void CS_GetSourceVideoMode(CS_Source source, CS_VideoMode* mode,
                           CS_Status* status);
CS_Bool CS_SetSourceVideoMode(CS_Source source, const CS_VideoMode* mode,
                              CS_Status* status);
CS_Bool CS_SetSourceVideoModeDiscrete(CS_Source source,
                                      enum CS_PixelFormat pixelFormat,
                                      int width, int height, int fps,
                                      CS_Status* status);
CS_Bool CS_SetSourcePixelFormat(CS_Source source,
                                enum CS_PixelFormat pixelFormat,
                                CS_Status* status);
CS_Bool CS_SetSourceResolution(CS_Source source, int width, int height,
                               CS_Status* status);
CS_Bool CS_SetSourceFPS(CS_Source source, int fps, CS_Status* status);
CS_VideoMode* CS_EnumerateSourceVideoModes(CS_Source source, int* count,
                                           CS_Status* status);
CS_Source CS_CopySource(CS_Source source, CS_Status* status);
void CS_ReleaseSource(CS_Source source, CS_Status* status);

//
// OpenCV Source Functions
//
void CS_PutSourceFrame(CS_Source source, struct CvMat* image,
                       CS_Status* status);
void CS_NotifySourceError(CS_Source source, const char* msg, CS_Status* status);
void CS_SetSourceConnected(CS_Source source, CS_Bool connected,
                           CS_Status* status);
void CS_SetSourceDescription(CS_Source source, const char* description,
                             CS_Status* status);
CS_Property CS_CreateSourceProperty(CS_Source source, const char* name,
                                    enum CS_PropertyType type, int minimum,
                                    int maximum, int step, int defaultValue,
                                    int value, CS_Status* status);
void CS_SetSourceEnumPropertyChoices(CS_Source source, CS_Property property,
                                     const char** choices, int count,
                                     CS_Status* status);

//
// Sink Creation Functions
//
CS_Sink CS_CreateMJPEGServer(const char* name, const char* listenAddress,
                             int port, CS_Status* status);
CS_Sink CS_CreateCvSink(const char* name, CS_Status* status);
CS_Sink CS_CreateCvSinkCallback(const char* name, void* data,
                                void (*processFrame)(void* data, uint64_t time),
                                CS_Status* status);

//
// Sink Functions
//
char* CS_GetSinkName(CS_Sink sink, CS_Status* status);
char* CS_GetSinkDescription(CS_Sink sink, CS_Status* status);
void CS_SetSinkSource(CS_Sink sink, CS_Source source, CS_Status* status);
CS_Property CS_GetSinkSourceProperty(CS_Sink sink, const char* name,
                                     CS_Status* status);
CS_Source CS_GetSinkSource(CS_Sink sink, CS_Status* status);
CS_Sink CS_CopySink(CS_Sink sink, CS_Status* status);
void CS_ReleaseSink(CS_Sink sink, CS_Status* status);

//
// OpenCV Sink Functions
//
void CS_SetSinkDescription(CS_Sink sink, const char* description,
                           CS_Status* status);
uint64_t CS_GrabSinkFrame(CS_Sink sink, struct CvMat* image, CS_Status* status);
char* CS_GetSinkError(CS_Sink sink, CS_Status* status);
void CS_SetSinkEnabled(CS_Sink sink, CS_Bool enabled, CS_Status* status);

//
// Listener Functions
//
void CS_SetListenerOnStart(void (*onStart)(void* data), void* data);
void CS_SetListenerOnExit(void (*onExit)(void* data), void* data);
CS_Listener CS_AddListener(void* data,
                           void (*callback)(void* data, const CS_Event* event),
                           int eventMask, int immediateNotify,
                           CS_Status* status);

void CS_RemoveListener(CS_Listener handle, CS_Status* status);

int CS_NotifierDestroyed(void);

//
// Utility Functions
//
typedef struct CS_USBCameraInfo {
  int dev;
  char* path;
  char* name;
} CS_USBCameraInfo;

CS_USBCameraInfo* CS_EnumerateUSBCameras(int* count, CS_Status* status);
void CS_FreeEnumeratedUSBCameras(CS_USBCameraInfo* cameras, int count);

CS_Source* CS_EnumerateSources(int* count, CS_Status* status);
void CS_ReleaseEnumeratedSources(CS_Source* sources, int count);

CS_Sink* CS_EnumerateSinks(int* count, CS_Status* status);
void CS_ReleaseEnumeratedSinks(CS_Sink* sinks, int count);

void CS_FreeString(char* str);
void CS_FreeEnumPropertyChoices(char** choices, int count);

void CS_FreeEnumeratedProperties(CS_Property* properties, int count);
void CS_FreeEnumeratedVideoModes(CS_VideoMode* modes, int count);

#ifdef __cplusplus
}
#endif

#endif /* CSCORE_C_H_ */