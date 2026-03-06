#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // กรณีที่ไม่มีการใส่ตัวเลขตามหลังคำสั่ง (มีแค่ชื่อโปรแกรม)
    if (argc == 1) {
        std::cout << "Please input numbers to find average." << std::endl;
        return 0;
    }

    // จำนวนตัวเลขที่รับเข้ามา (หักชื่อโปรแกรมใน argv[0] ออก)
    int count = argc - 1;
    double sum = 0.0;

    // วนลูปนำค่าตั้งแต่ argv[1] ถึงตัวสุดท้าย มาบวกเข้ากับ sum
    for (int i = 1; i < argc; ++i) {
        try {
            // แปลงข้อความ (String) ให้เป็นตัวเลขทศนิยม (Double)
            sum += std::stod(argv[i]);
        } catch (const std::exception& e) {
            std::cout << "Error: Invalid input '" << argv[i] << "' is not a number." << std::endl;
            return 1;
        }
    }

    // คำนวณค่าเฉลี่ย
    double average = sum / count;

    // แสดงผลลัพธ์ตามรูปแบบที่กำหนด
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Average of " << count << " numbers = " << average << std::endl;
    std::cout << "---------------------------------" << std::endl;

    return 0;
}
