## Giới thiệu ##

Tài liệu này mô tả cách thức đặt tên, chú giải, trình bày và định dạng mã nguồn


## Chi tiết ##

### Quy tắc đặt tên ###
#### Tên lớp ####
  * Sử dụng chữ cái in hoa như dấu cách, phần còn lại chữ thường
  * Ký tự đầu tiên phải viết hoa
  * Không sử dụng dấu gạch dưới ('_')
  *_Ví dụ_: BigDog hoặc Character_

#### Tên phương thức ####
  * Áp dụng quy tắc giống tên lớp **trừ việc chữ cái đầu tiên viết thường**
  * _Ví dụ_: doIt(), handleError();

#### Tên biến ####
  * Áp dụng quy tắc giống tên lớp **trừ việc chữ cái đầu tiên viết thường**
  * _Ví dụ_: currentHealth, lastLevel

#### Tên con trỏ ####
  * tên con trỏ **nên** được bắt đầu bằng 'p'
  * Vị trí đặt dấu **ở gần kiểu con trỏ hơn là biến
  * _Ví dụ_: Health** currentHealth, Health**pCurrentHealth là đúng, Health**currentHealth là sai

#### Tệp mã nguồn ####
  * Tên tệp mã nguồn đặt trùng tên lớp
  * Tệp mã nguồn gồm tệp
    1. Tệp .h cho include và khai báo nguyên mẫu hàm, phương thức, thuộc tính
    1. Tệp .cpp cho triển khai cụ thể các phương thức

#### Quy tắc ghi chú thích ####
  * Phần đầu mỗi file
```
/**
 * \Summary: Game configuration manager and helper
 * \Filename: configuration.h
 * \Encoding: UTF-8
 * \Tabsize: 8
 * \Indentation: 4
 * \CreatedDate: 18:33 2008/05/26
 * \InitializedBy: Bob
 * \CreatedBy: FOSP Team
 * \Copyright: FOS Project
 */
```

  * Quy tắc #ifndef đối với file .h phải có
```
#ifndef C_CHARACTER_H_
#define C_CHARACTER_H_

//file content

#endif /*C_CHARACTER_H_*/
```

  * Quy tắc ghi chú thích bất kì
```
//! constructor

//! destructor
```

  * Quy tắc ghi chú thích một hàm
```
/**
 * \summary Finds a game state by its name.
 * \param projection: The new projection matrix of the camera. 
 * \return Int value, 0 if no such game state exits.
 */ 
virtual int setProjectionMatrix(const core::matrix4& projection);
```

  * Quy tắc định nghĩa todo, task
```
//! TODO : mouse control can rotate player and camera at higher speed
//! TASK : edit this source code's style
```