#!/bin/bash
# 编译环形缓冲区 C 扩展模块

echo "开始编译环形缓冲区扩展模块..."

# 检查 Python 是否可用
if ! command -v python3 &> /dev/null; then
    echo "错误: 未找到 python3"
    exit 1
fi

# 编译扩展模块
python3 setup.py build_ext --inplace

if [ $? -eq 0 ]; then
    echo "编译成功！"
    echo ""
    echo "现在可以运行测试:"
    echo "  python3 tests/test_ringbuffer.py"
    echo ""
    echo "或运行示例:"
    echo "  python3 example.py"
else
    echo "编译失败！"
    exit 1
fi

