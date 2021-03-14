#define PY_SSIZE_T_CLEAN
#pragma once
#include <Python.h>

/*
 *************************************************
 * 
 * general decoder function headers
 * 
 ************************************************
*/
#include "ProcessDxtc.hpp"
#include "ProcessRGB.hpp"

static PyObject *compress(PyObject *self, PyObject *args, uint8_t pixel_per_byte,
    void (*func)(const uint32_t* src, uint64_t* dst, uint32_t blocks, size_t width));

static PyObject *compress_to_dxt1(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 8 bits w/o alpha
    // 8 bits per block / 16 pixel per block = 1/2 bytes per pixel 
    return compress(self, args, 2, CompressDxt1);
}

static PyObject *compress_to_dxt1_dither(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 8 bits w/o alpha
    // 8 bits per block / 16 pixel per block = 1/2 bytes per pixel 
    return compress(self, args, 2, CompressDxt1Dither);
}

static PyObject *compress_to_dxt5(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 16 bits w/ alpha
    // 16 bits per block / 16 pixel per block = 1 byte per pixel
    return compress(self, args, 1, CompressDxt5);
}


static PyObject *compress_to_etc1_alpha(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 16 bits w/ alpha
    // 16 bits per block / 16 pixel per block = 1 byte per pixel
    return compress(self, args, 1, CompressEtc1Alpha);
}

static PyObject *compress_to_etc2_alpha(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 16 bits w/ alpha
    // 16 bits per block / 16 pixel per block = 1 byte per pixel
    return compress(self, args, 1, CompressEtc2Alpha);
}

static PyObject *compress_to_etc1_rgb(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 8 bits w/ alpha
    // 8 bits per block / 16 pixel per block = 1/2 byte per pixel
    return compress(self, args, 2, CompressEtc1Rgb);
}

static PyObject *compress_to_etc1_rgb_dither(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 8 bits w/ alpha
    // 8 bits per block / 16 pixel per block = 1/2 byte per pixel
    return compress(self, args, 2, CompressEtc1RgbDither);
}

static PyObject *compress_to_etc2_rgb(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 16 bits w/ alpha
    // 16 bits per block / 16 pixel per block = 1 byte per pixel
    return compress(self, args, 2, CompressEtc2Rgb);
}

static PyObject *compress_to_etc2_rgba(PyObject *self, PyObject *args){
    // 4x4 block takes up 64 bits w/ alpha
    // after compression 16 bits w/ alpha
    // 16 bits per block / 16 pixel per block = 1 byte per pixel
    return compress(self, args, 1, CompressEtc2Rgba);
}

static PyObject *compress(PyObject *self, PyObject *args, uint8_t pixel_per_byte,
    void (*func)(const uint32_t* src, uint64_t* dst, uint32_t blocks, size_t width))
{
    // define vars
    const uint32_t *data;
    uint64_t data_size;
    uint32_t width, height;
    if (!PyArg_ParseTuple(args, "y#ii", &data, &data_size, &width, &height))
        return NULL;

    if ((width % 4 != 0) || (height % 4 != 0)){
        PyErr_SetString(PyExc_ValueError, "width or height not multiple of 4");
        assert(PyErr_Occurred());
        return NULL;
    }

    // reserve return data
    uint64_t buf_size = width * height / pixel_per_byte;
    uint64_t *buf = (uint64_t *)malloc(buf_size);

    if (buf == NULL)
        return PyErr_NoMemory();

    // compress
    func(data, buf, width * height / 16, width);

    // return
    PyObject *res = Py_BuildValue("y#", buf, buf_size);
    free(buf);
    return res;
}

/*
 *************************************************
 * 
 * python connection
 * 
 ************************************************
*/

// Exported methods are collected in a table
static struct PyMethodDef method_table[] = {
    {"compress_to_dxt1",
     (PyCFunction)compress_to_dxt1,
     METH_VARARGS,
     "Compresses RGBA to DXT1\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_dxt1_dither",
     (PyCFunction)compress_to_dxt1_dither,
     METH_VARARGS,
     "Compresses RGBA to DXT1 Dither\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_dxt5",
     (PyCFunction)compress_to_dxt5,
     METH_VARARGS,
     "Compresses RGBA to DXT5\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_etc1",
     (PyCFunction)compress_to_etc1_rgb,
     METH_VARARGS,
     "Compresses RGBA to ETC1 RGB\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_etc1_dither",
     (PyCFunction)compress_to_etc1_rgb_dither,
     METH_VARARGS,
     "Compresses RGBA to ETC1 Dither\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_etc1_alpha",
     (PyCFunction)compress_to_etc1_alpha,
     METH_VARARGS,
     "Compresses A to ETC1 Alpha\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_etc2_rgb",
     (PyCFunction)compress_to_etc2_rgb,
     METH_VARARGS,
     "Compresses RGBA to ETC2 RGB\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_etc2_rgba",
     (PyCFunction)compress_to_etc2_rgba,
     METH_VARARGS,
     "Compresses RGBA to ETC2 RGBA\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {"compress_to_etc2_alpha",
     (PyCFunction)compress_to_etc2_alpha,
     METH_VARARGS,
     "Compresses RGBA to ETC2 Alpha\
:param data: RGBA data of the image\
:type data: bytes\
:param width: width of the image\
:type width: int\
:param height: height of the image\
:type height: int\
:returns: compressed data\
:rtype: bytes"
},
    {NULL,
     NULL,
     0,
     NULL} // Sentinel value ending the table
};

// A struct contains the definition of a module
static PyModuleDef etcpak_module = {
    PyModuleDef_HEAD_INIT,
    "etcpak", // Module name
    "a python wrapper for Perfare's etcpak",
    -1, // Optional size of the module state memory
    method_table,
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

// The module init function
PyMODINIT_FUNC PyInit_etcpak(void)
{
    return PyModule_Create(&etcpak_module);
}