#pragma once
#include <Mui_Base.h>
using namespace Mui;
void AES256_CFB(UIResource src, _m_byte* out, _m_byte* key, _m_byte* iv, bool encrypt);