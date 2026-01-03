#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ringbuffer.h"

// RingBuffer 对象结构
typedef struct {
    PyObject_HEAD
    RingBuffer* rb;
} PyRingBuffer;

// 释放 RingBuffer 对象
static void PyRingBuffer_dealloc(PyRingBuffer* self) {
    if (self->rb) {
        ringbuffer_free(self->rb);
        self->rb = NULL;
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

// 初始化 RingBuffer 对象
static int PyRingBuffer_init(PyRingBuffer* self, PyObject* args, PyObject* kwds) {
    uint32_t capacity;
    
    if (!PyArg_ParseTuple(args, "I", &capacity)) {
        return -1;
    }
    
    if (capacity == 0) {
        PyErr_SetString(PyExc_ValueError, "capacity must be greater than 0");
        return -1;
    }
    
    self->rb = ringbuffer_new(capacity);
    if (!self->rb) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate ring buffer");
        return -1;
    }
    
    return 0;
}

// 检查是否为空
static PyObject* PyRingBuffer_is_empty(PyRingBuffer* self, PyObject* Py_UNUSED(ignored)) {
    if (ringbuffer_is_empty(self->rb)) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

// 检查是否已满
static PyObject* PyRingBuffer_is_full(PyRingBuffer* self, PyObject* Py_UNUSED(ignored)) {
    if (ringbuffer_is_full(self->rb)) {
        Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

// 获取大小
static PyObject* PyRingBuffer_size(PyRingBuffer* self, PyObject* Py_UNUSED(ignored)) {
    return PyLong_FromUnsignedLong(ringbuffer_size(self->rb));
}

// 获取容量
static PyObject* PyRingBuffer_capacity(PyRingBuffer* self, PyObject* Py_UNUSED(ignored)) {
    return PyLong_FromUnsignedLong(ringbuffer_capacity(self->rb));
}

// 推入元素
static PyObject* PyRingBuffer_push(PyRingBuffer* self, PyObject* args) {
    PyObject* item;
    
    if (!PyArg_ParseTuple(args, "O", &item)) {
        return NULL;
    }
    
    Py_INCREF(item);  // 增加引用计数
    
    if (!ringbuffer_push(self->rb, (void*)item)) {
        Py_DECREF(item);  // 如果失败，减少引用计数
        Py_RETURN_FALSE;
    }
    
    Py_RETURN_TRUE;
}

// 弹出元素
static PyObject* PyRingBuffer_pop(PyRingBuffer* self, PyObject* Py_UNUSED(ignored)) {
    PyObject* item = (PyObject*)ringbuffer_pop(self->rb);
    
    if (!item) {
        PyErr_SetString(PyExc_IndexError, "Ring buffer is empty");
        return NULL;
    }
    
    // 从缓冲区取出时，引用计数保持不变（因为我们之前 INCREF 过）
    // 返回给调用者，调用者会负责减少引用计数
    return item;
}

// 清空缓冲区
static PyObject* PyRingBuffer_clear(PyRingBuffer* self, PyObject* Py_UNUSED(ignored)) {
    // 先释放所有 Python 对象的引用
    while (!ringbuffer_is_empty(self->rb)) {
        PyObject* item = (PyObject*)ringbuffer_pop(self->rb);
        if (item) {
            Py_DECREF(item);
        }
    }
    
    ringbuffer_clear(self->rb);
    Py_RETURN_NONE;
}

// 方法定义
static PyMethodDef PyRingBuffer_methods[] = {
    {"is_empty", (PyCFunction)PyRingBuffer_is_empty, METH_NOARGS, "Check if the ring buffer is empty"},
    {"is_full", (PyCFunction)PyRingBuffer_is_full, METH_NOARGS, "Check if the ring buffer is full"},
    {"size", (PyCFunction)PyRingBuffer_size, METH_NOARGS, "Get the number of elements in the buffer"},
    {"capacity", (PyCFunction)PyRingBuffer_capacity, METH_NOARGS, "Get the capacity of the buffer"},
    {"push", (PyCFunction)PyRingBuffer_push, METH_VARARGS, "Push an item into the buffer"},
    {"pop", (PyCFunction)PyRingBuffer_pop, METH_NOARGS, "Pop an item from the buffer"},
    {"clear", (PyCFunction)PyRingBuffer_clear, METH_NOARGS, "Clear all items from the buffer"},
    {NULL, NULL, 0, NULL}
};

// 类型定义
static PyTypeObject PyRingBufferType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "ringbuffer.RingBuffer",
    .tp_doc = "Ring buffer implemented in C",
    .tp_basicsize = sizeof(PyRingBuffer),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyRingBuffer_init,
    .tp_dealloc = (destructor)PyRingBuffer_dealloc,
    .tp_methods = PyRingBuffer_methods,
};

// 模块定义
static struct PyModuleDef ringbuffermodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "ringbuffer",
    .m_doc = "A high-performance ring buffer implemented in C",
    .m_size = -1,
};

// 模块初始化函数
PyMODINIT_FUNC PyInit_ringbuffer(void) {
    PyObject* m;
    
    if (PyType_Ready(&PyRingBufferType) < 0) {
        return NULL;
    }
    
    m = PyModule_Create(&ringbuffermodule);
    if (m == NULL) {
        return NULL;
    }
    
    Py_INCREF(&PyRingBufferType);
    if (PyModule_AddObject(m, "RingBuffer", (PyObject*)&PyRingBufferType) < 0) {
        Py_DECREF(&PyRingBufferType);
        Py_DECREF(m);
        return NULL;
    }
    
    return m;
}

