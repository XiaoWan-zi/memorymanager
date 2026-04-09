from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os

# 获取模块目录路径
module_dir = os.path.join(os.path.dirname(__file__), 'src', 'modules')

# 定义 C 扩展模块
ringbuffer_module = Extension(
    'ringbuffer',
    sources=[
        os.path.join(module_dir, 'ringbuffermodule.c'),
        os.path.join(module_dir, 'ringbuffer.c'),
    ],
    include_dirs=[module_dir],
    extra_compile_args=['-O2', '-Wall'],
    language='c',
)

setup(
    name='memorymanager',
    version='0.1.0',
    description='A high-performance ring buffer implemented in CPython C extension',
    ext_modules=[ringbuffer_module],
    zip_safe=False,
    python_requires='>=3.6',
)


