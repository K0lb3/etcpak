#define PY_SSIZE_T_CLEAN
#pragma once
#include <Python.h>
#include "pybc7params.hpp"

/*
 *************************************************
 *
 * general decoder function headers
 *
 ************************************************
 */
#include "ProcessDxtc.hpp"
#include "ProcessRGB.hpp"

template <void (*CompressEtc2Func)(const uint32_t *src, uint64_t *dst, uint32_t blocks, size_t width, bool useHeuristics), bool USE_HEURISTICS>
void CompressEtc2(const uint32_t *src, uint64_t *dst, uint32_t blocks, size_t width)
{
    return CompressEtc2Func(src, dst, blocks, width, USE_HEURISTICS);
}

// generic compress function
typedef void (*CompressFunc)(const uint32_t *src, uint64_t *dst, uint32_t blocks, size_t width);
template <CompressFunc Func, uint8_t PIXEL_PER_BYTE, bool RGBA_TO_BGRA>
static PyObject *compress(PyObject *self, PyObject *args)
{
    // define vars
    uint32_t *data;
    uint64_t data_size;
    uint32_t width, height;
    if (!PyArg_ParseTuple(args, "y#ii", &data, &data_size, &width, &height))
        return NULL;

    if ((width % 4 != 0) || (height % 4 != 0))
    {
        PyErr_SetString(PyExc_ValueError, "width or height not multiple of 4");
        assert(PyErr_Occurred());
        return NULL;
    }

    // etc expects bgra instead of rgba, so we need to swap the bytes
    if (RGBA_TO_BGRA)
    {
        for (uint64_t i = 0; i < width * height; i++)
        {
            uint32_t pixel = data[i];
            data[i] = (pixel & 0xFF00FF00) | ((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16);
        }
    }

    // reserve return data
    uint64_t buf_size = width * height / PIXEL_PER_BYTE;
    uint64_t *buf = (uint64_t *)malloc(buf_size);

    if (buf == NULL)
        return PyErr_NoMemory();

    // compress
    Func(data, buf, width * height / 16, width);

    // return
    PyObject *res = Py_BuildValue("y#", buf, buf_size);
    free(buf);
    return res;
}

// bc7 compress
static PyObject *compress_bc7(PyObject *self, PyObject *args)
{
    // define vars
    const uint32_t *data;
    uint64_t data_size;
    uint32_t width, height;
    PyObject *params = nullptr;
    if (!PyArg_ParseTuple(args, "y#ii|O", &data, &data_size, &width, &height, &params))
        return NULL;

    if ((width % 4 != 0) || (height % 4 != 0))
    {
        PyErr_SetString(PyExc_ValueError, "width or height not multiple of 4");
        assert(PyErr_Occurred());
        return NULL;
    }

    // reserve return data
    uint64_t buf_size = width * height;
    uint64_t *buf = (uint64_t *)malloc(buf_size);

    if (buf == NULL)
        return PyErr_NoMemory();

    // compress
    bc7enc_compress_block_init();
    if (params == nullptr)
    {
        bc7enc_compress_block_params bc7params;
        bc7enc_compress_block_params_init(&bc7params);
        CompressBc7(data, buf, width * height / 16, width, &bc7params);
    }
    else
    {
        if (!PyObject_IsInstance(params, (PyObject *)&PyBC7CompressBlockParamsType))
        {
            PyErr_SetString(PyExc_ValueError, "params must be an instance of BC7CompressBlockParams");
            free(buf);
            assert(PyErr_Occurred());
            return NULL;
        }
        CompressBc7(data, buf, width * height / 16, width, &((PyBC7CompressBlockParams *)params)->params);
    }
    // return
    PyObject *res = Py_BuildValue("y#", buf, buf_size);
    free(buf);
    return res;
}

// stupid hack to access protected members
#define _ALLOW_KEYWORD_MACROS
#define protected public
#define private public
#include "BlockData.hpp"

template <BlockData::Type blockDataType>
static PyObject *decompress(PyObject *self, PyObject *args)
{
    const uint32_t *data;
    uint64_t data_size;
    uint32_t width, height;
    if (!PyArg_ParseTuple(args, "y#ii", &data, &data_size, &width, &height))
        return NULL;

    if ((width % 4 != 0) || (height % 4 != 0))
    {
        PyErr_SetString(PyExc_ValueError, "width or height not multiple of 4");
        assert(PyErr_Occurred());
        return NULL;
    }

    v2i size(width, height);
    BlockData blockData(size, false, blockDataType);
    memcpy(
        blockData.m_data + blockData.m_dataOffset,
        data,
        data_size);

    BitmapPtr decodedBitmap = blockData.Decode();

    PyObject *res = Py_BuildValue(
        "y#",
        decodedBitmap->m_data,
        decodedBitmap->m_size.x * decodedBitmap->m_size.y * 4);

    return res;
}
#undef protected
#undef private
#undef _ALLOW_KEYWORD_MACROS
//

/*
 *************************************************
 *
 * python connection
 *
 ************************************************
 */

// Exported methods are collected in a table
static struct PyMethodDef method_table[] = {
    {"compress_bc1",
     (PyCFunction)compress<CompressBc1, 2, false>,
     METH_VARARGS,
     ""},
    {"compress_bc1_dither",
     (PyCFunction)compress<CompressBc1Dither, 2, false>,
     METH_VARARGS,
     ""},
    {"compress_bc3",
     (PyCFunction)compress<CompressBc3, 1, false>,
     METH_VARARGS,
     ""},
    {"compress_bc4",
     (PyCFunction)compress<CompressBc4, 2, false>,
     METH_VARARGS,
     ""},
    {"compress_bc5",
     (PyCFunction)compress<CompressBc5, 1, false>,
     METH_VARARGS,
     ""},
    {"compress_bc7",
     (PyCFunction)compress_bc7,
     METH_VARARGS,
     ""},
    {"compress_etc1_rgb",
     (PyCFunction)compress<CompressEtc1Rgb, 2, true>,
     METH_VARARGS,
     ""},
    {"compress_etc1_rgb_dither",
     (PyCFunction)compress<CompressEtc1RgbDither, 2, true>,
     METH_VARARGS,
     ""},
    {"compress_etc2_rgb",
     (PyCFunction)compress<CompressEtc2<CompressEtc2Rgb, true>, 2, true>,
     METH_VARARGS,
     ""},
    {"compress_etc2_rgba",
     (PyCFunction)compress<CompressEtc2<CompressEtc2Rgba, true>, 1, true>,
     METH_VARARGS,
     ""},
    {"compress_eac_r",
     (PyCFunction)compress<CompressEacR, 1, true>,
     METH_VARARGS,
     ""},
    {"compress_eac_rg",
     (PyCFunction)compress<CompressEacRg, 1, true>,
     METH_VARARGS,
     ""},
    {"decompress_etc1_rgb",
     (PyCFunction)decompress<BlockData::Etc1>,
     METH_VARARGS,
     ""},
    {"decompress_etc2_rgb",
     (PyCFunction)decompress<BlockData::Etc2_RGB>,
     METH_VARARGS,
     ""},
    {"decompress_etc2_rgba",
     (PyCFunction)decompress<BlockData::Etc2_RGBA>,
     METH_VARARGS,
     ""},
    {"decompress_etc2_r11",
     (PyCFunction)decompress<BlockData::Etc2_R11>,
     METH_VARARGS,
     ""},
    {"decompress_etc2_rg11",
     (PyCFunction)decompress<BlockData::Etc2_RG11>,
     METH_VARARGS,
     ""},
    {"decompress_bc1",
     (PyCFunction)decompress<BlockData::Bc1>,
     METH_VARARGS,
     ""},
    {"decompress_bc3",
     (PyCFunction)decompress<BlockData::Bc3>,
     METH_VARARGS,
     ""},
    {"decompress_bc4",
     (PyCFunction)decompress<BlockData::Bc4>,
     METH_VARARGS,
     ""},
    {"decompress_bc5",
     (PyCFunction)decompress<BlockData::Bc5>,
     METH_VARARGS,
     ""},
    {"decompress_bc7",
     (PyCFunction)decompress<BlockData::Bc7>,
     METH_VARARGS,
     ""},
    {NULL,
     NULL,
     0,
     NULL} // Sentinel value ending the table
};

// A struct contains the definition of a module
static PyModuleDef etcpak_module = {
    PyModuleDef_HEAD_INIT,
    MODULE_NAME, //"_etcpak", // Module name
    "a python wrapper for Perfare's etcpak",
    -1, // Optional size of the module state memory
    method_table,
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

static void add_type(PyObject *m, PyTypeObject *obj, const char *name)
{
    if (PyType_Ready(obj) < 0)
        return;
    Py_INCREF(obj);
    PyModule_AddObject(m, name, (PyObject *)obj);
}

// The module init function
PyMODINIT_FUNC INIT_FUNC_NAME(void)
{
    PyObject *m = PyModule_Create(&etcpak_module);
    if (m == NULL)
        return NULL;
    add_type(m, &PyBC7CompressBlockParamsType, "BC7CompressBlockParams");
    return m;
}