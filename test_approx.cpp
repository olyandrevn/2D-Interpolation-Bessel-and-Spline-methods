#include "test_approx.h"
#include "window.h"
#include <QMainWindow>

void TestApprox::testZero()
{
    QMainWindow main;
    QLabel label1;
    QLabel label2;
    Window window(&main, &label1, &label2);

    QTest::keySequence(&window, QKeySequence(Qt::CTRL + Qt::Key_C));
    QTest::keySequence(&window, QKeySequence(Qt::CTRL + Qt::Key_X));
}

QTEST_MAIN(TestApprox)
