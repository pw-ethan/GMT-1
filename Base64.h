/*******************************************************************
 *  文件名称: Base64.h
 *  简要描述: Base64编码
 *
 *  创建日期: 2016年10月14日
 *  作者: pw-ethan
 *  说明:.
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#pragma once

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

