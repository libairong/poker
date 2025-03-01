/*================================================================
*   Copyright (C) 2025 Ltd. All rights reserved.
*
*   file        : main.cpp
*   owner       : bairong.li
*   date        : 2025-02-09
*   description :
*
================================================================*/

#ifdef __DISPLAY_DEBUG__
#include "Layer.h"
#include "TerminalDisplay.h"

class A : public Layer {
public:
    A() : Layer(10, 5) {
        // cout << "A()" << endl;
    }
    ~A() {
        // cout << "~A()" << endl;
    }

    void init() {
        cout << "init()" << endl;
        cout << getName() << endl;
        setContentString(0, 0, getName(), Color::RED);

    }

};

int main() {
    // 创建两个图层
    auto layer1 = make_shared<Layer>(10, 5); // 创建一个10x5的图层
    layer1->setStartPosition(0, 0);
    layer1->setName("layer1");
    auto layer2 = make_shared<Layer>(10, 5); // 创建另一个10x5的图层
    layer2->setStartPosition(10, 5);
    layer2->setName("layer2");

    // 向图层1添加一些内容，并设置颜色
    layer1->setContent(1, 1, layer1->getName(), Color::RED, {Color::BLINK});
    layer1->setContent(2, 2, 'O', Color::GREEN);

    // 向图层2添加一些内容，并设置颜色
    layer2->setContent(0, 0, layer2->getName(), Color::BLUE, {Color::UNDERLINE});
    layer2->setContent(9, 4, '@', Color::MAGENTA);
    // 创建图层管理器并添加图层

    auto& terminalDisplay = TerminalDisplay::getInstance();
    terminalDisplay.addLayer(layer1);
    terminalDisplay.addLayer(layer2);

    auto a = make_shared<A>();
    a->setName("♦");
    a->init();
    a->setStartPosition(20, 5);
    std::shared_ptr<Layer> parentPtr = std::dynamic_pointer_cast<Layer>(a);
    terminalDisplay.addLayer(parentPtr);
    // 显示图层数量
    cout << "图层数量：" << terminalDisplay.getLayerCount() << endl;
    // 显示所有图层的内容
    cout << "显示所有图层内容：\n";
    // terminalDisplay.displayAll();

    // 清空所有图层内容
    // terminalDisplay.clearAll();

    // 停止等待用户输入，以便查看结果
    // terminalDisplay.overlayAndDisplay();
    cout << "按任意键继续..." << endl;
    getchar();

    cout << "\n所有图层内容已清空！\n";

    return 0;
}
#endif
