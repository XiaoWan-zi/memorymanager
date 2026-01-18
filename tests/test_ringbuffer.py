"""
测试环形缓冲区的基本功能
"""
import sys
import os

# 添加项目路径到 sys.path
project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(project_root, 'src', 'modules'))


try:
    from ringbuffer import RingBuffer
except ImportError:
    print("错误: 无法导入 ringbuffer 模块")
    print("请先运行: python setup.py build_ext --inplace")
    sys.exit(1)


def test_basic_operations():
    """测试基本操作"""
    print("测试基本操作...")
    rb = RingBuffer(8)
    
    # 测试初始状态
    assert rb.is_empty() == True
    assert rb.is_full() == False
    assert rb.size() == 0
    assert rb.capacity() == 8
    
    # 测试 push
    assert rb.push(1) == True
    assert rb.push(2) == True
    assert rb.push(3) == True
    assert rb.size() == 3
    assert rb.is_empty() == False
    
    # 测试 pop
    assert rb.pop() == 1
    assert rb.pop() == 2
    assert rb.size() == 1
    
    # 测试清空
    rb.clear()
    assert rb.is_empty() == True
    print("✓ 基本操作测试通过")


def test_full_buffer():
    """测试缓冲区满的情况"""
    print("测试缓冲区满的情况...")
    rb = RingBuffer(4)
    
    # 填充缓冲区（容量4，实际可存3个，因为留一个位置区分空和满）
    assert rb.push(1) == True
    assert rb.push(2) == True
    assert rb.push(3) == True
    assert rb.is_full() == True
    assert rb.size() == 3
    
    # 缓冲区满时，push 会覆盖最旧的数据，并返回 True
    assert rb.push(4) == True  # 会覆盖最旧的数据（1）
    assert rb.is_full() == True
    assert rb.size() == 3  # 大小保持不变
    
    # 验证最旧的数据（1）被覆盖了，现在应该弹出 2, 3, 4
    assert rb.pop() == 2
    assert rb.pop() == 3
    assert rb.pop() == 4
    assert rb.is_empty() == True
    
    # 弹出后可以继续写入
    assert rb.push(5) == True
    assert rb.is_full() == False
    assert rb.push(6) == True
    print("✓ 缓冲区满测试通过")


def test_empty_buffer():
    """测试空缓冲区"""
    print("测试空缓冲区...")
    rb = RingBuffer(4)
    
    try:
        rb.pop()
        assert False, "应该抛出 IndexError"
    except IndexError:
        pass
    
    print("✓ 空缓冲区测试通过")


def test_wrap_around():
    """测试环形回绕"""
    print("测试环形回绕...")
    rb = RingBuffer(4)
    
    # 写入3个
    rb.push(1)
    rb.push(2)
    rb.push(3)
    
    # 弹出2个
    assert rb.pop() == 1
    assert rb.pop() == 2
    
    # 再写入2个，应该能回绕
    assert rb.push(4) == True
    assert rb.push(5) == True
    
    # 应该能正确读取
    assert rb.pop() == 3
    assert rb.pop() == 4
    assert rb.pop() == 5
    assert rb.is_empty() == True
    
    print("✓ 环形回绕测试通过")


def test_capacity_power_of_two():
    """测试容量自动调整为2的幂"""
    print("测试容量调整...")
    # 创建容量为5的缓冲区，应该自动调整为8
    rb = RingBuffer(5)
    assert rb.capacity() == 8
    
    rb = RingBuffer(10)
    assert rb.capacity() == 16
    
    rb = RingBuffer(16)
    assert rb.capacity() == 16
    
    print("✓ 容量调整测试通过")


if __name__ == '__main__':
    print("开始测试环形缓冲区...\n")
    
    try:
        test_basic_operations()
        test_full_buffer()
        test_empty_buffer()
        test_wrap_around()
        test_capacity_power_of_two()
        
        print("\n所有测试通过！✓")
    except Exception as e:
        print(f"\n测试失败: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)



