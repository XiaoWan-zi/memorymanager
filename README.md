# memorymanager

一个使用 CPython C 扩展实现的高性能环形缓冲区（Ring Buffer）库。

## 特性

- 使用 C 语言实现，性能优异
- 容量自动调整为 2 的幂，使用位运算优化
- 支持基本的 push/pop 操作
- 线程安全的读写操作（单生产者单消费者场景）
- 完整的 Python 绑定

## 编译安装

### 编译扩展模块

```bash
# 在项目根目录下
python setup.py build_ext --inplace
```

或者使用 pip 安装：

```bash
pip install -e .
```

## 使用方法

```python
from ringbuffer import RingBuffer

# 创建容量为 8 的环形缓冲区
rb = RingBuffer(8)

# 推入元素
rb.push(1)
rb.push(2)
rb.push(3)

# 检查状态
print(rb.size())      # 输出: 3
print(rb.is_empty())  # 输出: False
print(rb.is_full())   # 输出: False

# 弹出元素
item = rb.pop()       # 返回: 1
item = rb.pop()       # 返回: 2

# 清空缓冲区
rb.clear()
```

## API 文档

### RingBuffer(capacity)

创建环形缓冲区对象。

- `capacity`: 缓冲区容量（会自动调整为 2 的幂）

### 方法

- `push(item)`: 推入一个元素到缓冲区，返回 `True` 成功，`False` 表示缓冲区已满
- `pop()`: 从缓冲区弹出一个元素，如果缓冲区为空会抛出 `IndexError`
- `is_empty()`: 检查缓冲区是否为空
- `is_full()`: 检查缓冲区是否已满
- `size()`: 返回当前缓冲区中的元素数量
- `capacity()`: 返回缓冲区的容量
- `clear()`: 清空缓冲区中的所有元素

## 运行测试

```bash
python tests/test_ringbuffer.py
```

## 运行示例

```bash
python example.py
```

## 实现细节

本实现参考了 viztracer 的设计思路：

1. **容量优化**: 容量必须是 2 的幂，这样可以使用位运算 `&` 代替取模运算，提高性能
2. **内存布局**: 使用连续内存存储，提高缓存局部性
3. **无锁设计**: 使用 volatile 关键字标记读写位置，适合单生产者单消费者场景

## 文件结构

```
memorymanager/
├── src/
│   └── modules/
│       ├── ringbuffer.h          # C 头文件
│       ├── ringbuffer.c          # C 实现
│       ├── ringbuffermodule.c    # Python C 扩展绑定
│       └── __init__.py           # Python 模块初始化
├── tests/
│   └── test_ringbuffer.py        # 测试文件
├── example.py                     # 使用示例
├── setup.py                       # 编译配置
└── README.md                      # 本文件
```
