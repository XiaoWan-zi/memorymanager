"""
Ring Buffer Module

A high-performance ring buffer implementation using CPython C extension.
"""

try:
    # 尝试导入编译后的 C 扩展模块
    import ringbuffer
    RingBuffer = ringbuffer.RingBuffer
    __all__ = ['RingBuffer']
except ImportError:
    # 如果导入失败，可能是还没有编译
    __all__ = []
    RingBuffer = None

