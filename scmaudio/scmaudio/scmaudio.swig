%module ScmAudio
%{
#include "session.h"
%}

%include <windows.i>
%include "std_string.i"
%include "std_vector.i"
%include "session.h"

%template(DeviceList) std::vector<Device>;