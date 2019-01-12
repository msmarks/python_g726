#include "Python.h"
#include "g72x.h"



typedef struct {
    PyObject_HEAD
    int coding;
    int g726_bits;
    g726_state state_ptr;
} G726_cvObject;

static PyObject *g726_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    G726_cvObject *self;
    // we will free self in function g726_dealloc later
    self = (G726_cvObject *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

// init(int coding, int g726_bits)
static int g726_init(G726_cvObject *self, PyObject *args, PyObject *kwds) {
    int coding, g726_bits;

    int succeed = PyArg_ParseTuple(args, "ii", &coding, &g726_bits);

    if(succeed){
        g726_init_state(&(self->state_ptr));
        self->coding = coding;
        self->g726_bits = g726_bits;

        return 0;
    }
}

static void g726_dealloc(G726_cvObject* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

/*
static PyMemberDef g726_members[] = {
    {NULL}
}*/


static PyObject *py_g726_to_pcm(G726_cvObject *self, PyObject *args) {
    char* buf = NULL;
    Py_ssize_t buf_len;

    short *buf2;
    char *buf3;
    size_t i, a, result_len;

    int succeed = PyArg_ParseTuple(args, "y#", &buf, &buf_len);
    if(succeed) {
        /* cast Py_ssize_t signed integer to unsigned */
        size_t size_t_MAX = -1;
        size_t unsigned_buf_len = buf_len + size_t_MAX + 1;

        switch(self->g726_bits){
            case 16:
                result_len = unsigned_buf_len / 4 * 16;
                buf2 = malloc(result_len);
                for(i = 0; i < unsigned_buf_len; i+= 4){
                    a = i / 4 * 16;
                    buf2[a + 0] = g726_16_decoder((buf[i] & 0xc0) >> 6, self->coding, &self->state_ptr);
                    buf2[a + 1] = g726_16_decoder((buf[i] & 0x30) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 2] = g726_16_decoder((buf[i] & 0x0c) >> 2, self->coding, &self->state_ptr);
                    buf2[a + 3] = g726_16_decoder((buf[i] & 0x03) >> 0, self->coding, &self->state_ptr);

                    buf2[a + 4] = g726_16_decoder((buf[i + 1] & 0xc0) >> 6, self->coding, &self->state_ptr);
                    buf2[a + 5] = g726_16_decoder((buf[i + 1] & 0x30) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 6] = g726_16_decoder((buf[i + 1] & 0x0c) >> 2, self->coding, &self->state_ptr);
                    buf2[a + 7] = g726_16_decoder((buf[i + 1] & 0x03) >> 0, self->coding, &self->state_ptr);

                    buf2[a + 8] = g726_16_decoder((buf[i + 2] & 0xc0) >> 6, self->coding, &self->state_ptr);
                    buf2[a + 9] = g726_16_decoder((buf[i + 2] & 0x30) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 10] = g726_16_decoder((buf[i + 2] & 0x0c) >> 2, self->coding, &self->state_ptr);
                    buf2[a + 11] = g726_16_decoder((buf[i + 2] & 0x03) >> 0, self->coding, &self->state_ptr);

                    buf2[a + 12] = g726_16_decoder((buf[i + 3] & 0xc0) >> 6, self->coding, &self->state_ptr);
                    buf2[a + 13] = g726_16_decoder((buf[i + 3] & 0x30) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 14] = g726_16_decoder((buf[i + 3] & 0x0c) >> 2, self->coding, &self->state_ptr);
                    buf2[a + 15] = g726_16_decoder((buf[i + 3] & 0x03) >> 0, self->coding, &self->state_ptr);
                }
                break;
            case 24:
                result_len = unsigned_buf_len / 4 * 10;
                buf2 = malloc(result_len);
                for(i = 0; i < unsigned_buf_len; i+= 4){
                    a = i / 4 * 10;
                    buf2[a + 0] = g726_24_decoder((buf[i] & 0x38) >> 3, self->coding, &self->state_ptr);
                    buf2[a + 1] = g726_24_decoder((buf[i] & 0x7) >> 0, self->coding, &self->state_ptr);
                    buf2[a + 2] = g726_24_decoder((buf[i + 1] & 0xe0) >> 5, self->coding, &self->state_ptr);
                    buf2[a + 3] = g726_24_decoder((buf[i + 1] & 0x1c) >> 2, self->coding, &self->state_ptr);
                    buf2[a + 4] = g726_24_decoder(((buf[i + 1] & 0x3) << 1) | ((buf[i + 2] & 0x80) >> 7), self->coding, &self->state_ptr);
                    buf2[a + 5] = g726_24_decoder((buf[i + 2] & 0x70) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 6] = g726_24_decoder((buf[i + 2] & 0xe) >> 1, self->coding, &self->state_ptr);
                    buf2[a + 7] = g726_24_decoder(((buf[i + 2] & 0x1) << 2) | ((buf[i + 3] & 0xc0) >> 6), self->coding, &self->state_ptr);
                    buf2[a + 8] = g726_24_decoder((buf[i + 3] & 0x38) >> 3, self->coding, &self->state_ptr);
                    buf2[a + 9] = g726_24_decoder((buf[i + 3] & 0x7) >> 0, self->coding, &self->state_ptr);
                 }
                break;
            case 32:
                result_len = unsigned_buf_len / 4 * 8;
                buf2 = malloc(result_len);
                for(i = 0; i < unsigned_buf_len; i+= 4){
                    a = i / 4 * 8;
                    buf2[a + 0] = g726_32_decoder((buf[i] & 0xf0) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 1] = g726_32_decoder((buf[i] & 0xf) >> 0, self->coding, &self->state_ptr);
                    buf2[a + 2] = g726_32_decoder((buf[i + 1] & 0xf0) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 3] = g726_32_decoder((buf[i + 1] & 0xf) >> 0, self->coding, &self->state_ptr);

                    buf2[a + 4] = g726_32_decoder((buf[i + 2] & 0xf0) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 5] = g726_32_decoder((buf[i + 2] & 0xf) >> 0, self->coding, &self->state_ptr);
                    buf2[a + 6] = g726_32_decoder((buf[i + 3] & 0xf0) >> 4, self->coding, &self->state_ptr);
                    buf2[a + 7] = g726_32_decoder((buf[i + 3] & 0xf) >> 0, self->coding, &self->state_ptr);
                }
                break;
            case 40:
                result_len = unsigned_buf_len / 4 * 6;
                buf2 = malloc(result_len);
                for(i = 0; i < unsigned_buf_len; i+= 4){
                    a = i / 4 * 6;
                    buf2[a + 0] = g726_40_decoder((buf[i] & 0x3e) >> 1, self->coding, &self->state_ptr);
                    buf2[a + 1] = g726_40_decoder(((buf[i] & 0x1) << 4) | ((buf[i + 1] & 0xf0) >> 4), self->coding, &self->state_ptr);
                    buf2[a + 2] = g726_40_decoder(((buf[i + 1] & 0xf) << 1) | ((buf[i + 2] & 0x80) >> 7), self->coding, &self->state_ptr);
                    buf2[a + 3] = g726_40_decoder((buf[i + 2] & 0x40) >> 2, self->coding, &self->state_ptr);

                    buf2[a + 4] = g726_40_decoder(((buf[i + 2] & 0x3) << 3) | ((buf[i + 3] & 0xe0) >> 5), self->coding, &self->state_ptr);
                    buf2[a + 5] = g726_40_decoder((buf[i + 2] & 0x1f) >> 0, self->coding, &self->state_ptr);
                }
                break;
            default:
                return NULL;

            buf3 = (char *)buf2;
            PyObject *myResult = Py_BuildValue("y#", buf3, result_len * 2);
            free(buf2);
            return myResult;
        }
    }
    return NULL;
}

/* py_pcm_to_g726() */
static PyObject *py_pcm_to_g726(G726_cvObject *self, PyObject *args) {
    char* buf = NULL;
    Py_ssize_t buf_len;

    // buf is managered by python, wen don't need to free
    int succeed = PyArg_ParseTuple(args, "y#", &buf, &buf_len);

    if(succeed){
        /* cast Py_ssize_t signed integer to unsigned */
        size_t size_t_MAX = -1;
        size_t unsigned_buf_len = buf_len + size_t_MAX + 1;

        short* buf_short;
        size_t i, a, result_len;
        char *buf2;
        int temp, temp2;

/*
16 16bits * 16 -> 2bits * 16 = 4bytes
b 0 b 0 b 1 b 1 b 2 b 2 b 3 b 3
b 4 b 4 b 5 b 5 b 6 b 6 b 7 b 7
b 8 b 8 b 9 b 9 b10 b10 b11 b11
b12 b12 b13 b13 b14 b14 b15 b15

24 16bits * 10 -> 3bits * 10 = 4bytes
00 00 b0 b0 b0 b1 b1 b1
b2 b2 b2 b3 b3 b3 b4 b4
b4 b5 b5 b5 b6 b6 b6 b7
b7 b7 b8 b8 b8 b9 b9 b9

32 16bits * 8 -> 4bits * 8 = 4bytes
b0 b0 b0 b0 b1 b1 b1 b1
b2 b2 b2 b2 b3 b3 b3 b3
b4 b4 b4 b4 b5 b5 b5 b5
b6 b6 b6 b6 b7 b7 b7 b7

40 16bits * 6 -> 5bits * 6 = 4bytes
00 00 b0 b0 b0 b0 b0 b1
b1 b1 b1 b1 b2 b2 b2 b2
b2 b3 b3 b3 b3 b3 b4 b4
b4 b4 b4 b5 b5 b5 b5 b5

*/

        switch(self->g726_bits){
            case 16:
                result_len = unsigned_buf_len >> 3;
                buf2 = malloc(result_len); // 2 ^ 3 = 8   16bits -> 2bits  32 char(16 short) -> 4char
                for(i = 0; i < unsigned_buf_len; i += 32){
                    buf_short = (short*)(buf + i); // 16 bits
                    a = i >> 3;
                    buf2[a] =
                        (g726_16_encoder(buf_short[0], self->coding, &self->state_ptr) << 6) |
                        (g726_16_encoder(buf_short[1], self->coding, &self->state_ptr) << 4) |
                        (g726_16_encoder(buf_short[2], self->coding, &self->state_ptr) << 2) |
                        (g726_16_encoder(buf_short[3], self->coding, &self->state_ptr) << 0);
                    buf2[a + 1] =
                        (g726_16_encoder(buf_short[4], self->coding, &self->state_ptr) << 6) |
                        (g726_16_encoder(buf_short[5], self->coding, &self->state_ptr) << 4) |
                        (g726_16_encoder(buf_short[6], self->coding, &self->state_ptr) << 2) |
                        (g726_16_encoder(buf_short[7], self->coding, &self->state_ptr) << 0);
                    buf2[a + 2] =
                        (g726_16_encoder(buf_short[8], self->coding, &self->state_ptr) << 6) |
                        (g726_16_encoder(buf_short[9], self->coding, &self->state_ptr) << 4) |
                        (g726_16_encoder(buf_short[10], self->coding, &self->state_ptr) << 2) |
                        (g726_16_encoder(buf_short[11], self->coding, &self->state_ptr) << 0);
                    buf2[a + 3] =
                        (g726_16_encoder(buf_short[12], self->coding, &self->state_ptr) << 6) |
                        (g726_16_encoder(buf_short[13], self->coding, &self->state_ptr) << 4) |
                        (g726_16_encoder(buf_short[14], self->coding, &self->state_ptr) << 2) |
                        (g726_16_encoder(buf_short[15], self->coding, &self->state_ptr) << 0);
                }
                break;
            case 24:
                result_len = unsigned_buf_len / 20 * 4;
                buf2 = malloc(result_len);  // 16bits -> 3bits  20 char(10 short) -> 4char
                for(i = 0; i < unsigned_buf_len; i += 20){
                    buf_short = (short*)(buf + i); // 16 bits
                    a = i / 20 * 4;
                    buf2[a] =
                        (g726_24_encoder(buf_short[0], self->coding, &self->state_ptr) << 3) |
                        (g726_24_encoder(buf_short[1], self->coding, &self->state_ptr) << 0) ;

                    temp = g726_24_encoder(buf_short[4], self->coding, &self->state_ptr);

                    buf2[a + 1] =
                        (g726_24_encoder(buf_short[2], self->coding, &self->state_ptr) << 5) |
                        (g726_24_encoder(buf_short[3], self->coding, &self->state_ptr) << 2) |
                        (temp >> 1);

                    temp2 = g726_24_encoder(buf_short[7], self->coding, &self->state_ptr) >> 2;

                    buf2[a + 2] =
                        (temp << 7) |
                        (g726_24_encoder(buf_short[5], self->coding, &self->state_ptr) << 4) |
                        (g726_24_encoder(buf_short[6], self->coding, &self->state_ptr) << 1) |
                        (temp2 >> 2);
                    buf2[a + 3] =
                        (temp2 << 6) |
                        (g726_24_encoder(buf_short[8], self->coding, &self->state_ptr) << 3) |
                        (g726_24_encoder(buf_short[9], self->coding, &self->state_ptr) << 0);
                }
                break;
            case 32:
                result_len = unsigned_buf_len / 16 * 4;
                buf2 = malloc(result_len);  // 16bits -> 4bits  16 char(8 short) -> 4char
                for(i = 0; i < unsigned_buf_len; i += 16){
                    buf_short = (short*)(buf + i);
                    a = i >> 2;
                    buf2[a] =
                        (g726_32_encoder(buf_short[0], self->coding, &self->state_ptr) << 4) |
                        (g726_32_encoder(buf_short[1], self->coding, &self->state_ptr) << 0);
                    buf2[a + 1] =
                        (g726_32_encoder(buf_short[2], self->coding, &self->state_ptr) << 4) |
                        (g726_32_encoder(buf_short[3], self->coding, &self->state_ptr) << 0);
                    buf2[a + 2] =
                        (g726_32_encoder(buf_short[4], self->coding, &self->state_ptr) << 4) |
                        (g726_32_encoder(buf_short[5], self->coding, &self->state_ptr) << 0);
                    buf2[a + 3] =
                        (g726_32_encoder(buf_short[6], self->coding, &self->state_ptr) << 4) |
                        (g726_32_encoder(buf_short[7], self->coding, &self->state_ptr) << 0);
                }
                break;
            case 40:
                result_len = unsigned_buf_len / 12 * 4;
                buf2 = malloc(result_len); // 16bits -> 5bits  12 char(4 short) -> 4char
                for(i = 0; i < unsigned_buf_len; i += 12){
                    buf_short = (short*)(buf + i);
                    a = i / 12 * 4;

                    temp = g726_40_encoder(buf_short[1], self->coding, &self->state_ptr);
                    temp2 = g726_40_encoder(buf_short[2], self->coding, &self->state_ptr);

                    buf2[a] =
                        (g726_40_encoder(buf_short[0], self->coding, &self->state_ptr) << 1) |
                        (temp >> 4);

                    buf2[a + 1] =
                        (temp << 4) |
                        (temp2 >> 1);

                    temp = g726_40_encoder(buf_short[4], self->coding, &self->state_ptr);

                    buf2[a + 2] =
                        (temp2 << 7) |
                        (g726_40_encoder(buf_short[3], self->coding, &self->state_ptr) << 2) |
                        (temp >> 3);

                    buf2[a + 3] =
                        (temp << 5) |
                        (g726_40_encoder(buf_short[5], self->coding, &self->state_ptr) << 0);
                }
                break;
            default:
                return NULL;
        }

        PyObject *myResult = Py_BuildValue("y#", buf2, result_len);
        free(buf2);
        return myResult;
    }
    return NULL;
}

static PyMethodDef g726_methods[] = {
    {"pcm_to_g726", (PyCFunction)py_pcm_to_g726, METH_VARARGS, "Convert pcm to g276" },
    {"g726_to_pcm", (PyCFunction)py_g726_to_pcm, METH_VARARGS, "Convert g276 to pcm" },
    {NULL}  /* Sentinel */
};

static PyTypeObject G726Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "G726",             /* tp_name */
    sizeof(G726_cvObject),   /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)g726_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "g726 objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    g726_methods,             /* tp_methods */
    0,//g726_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)g726_init,      /* tp_init */
    0,                         /* tp_alloc */
    g726_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

static struct PyModuleDef samplemodule = {
  PyModuleDef_HEAD_INIT,

  "g726",           /* name of module */
  "A sample module",  /* Doc string (may be NULL) */
  -1,                 /* Size of per-interpreter state or -1 */
  module_methods       /* Method table */
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_g726(void) {
    PyObject* m;

    if (PyType_Ready(&G726Type) < 0)
        return NULL;
    m = PyModule_Create(&samplemodule);

    if (m == NULL)
        return NULL;

    Py_INCREF(&G726Type);
    PyModule_AddObject(m, "g726", (PyObject *)&G726Type);

    PyModule_AddObject(m, "AUDIO_ENCODING_ULAW", Py_BuildValue("i", AUDIO_ENCODING_ULAW));
    PyModule_AddObject(m, "AUDIO_ENCODING_ALAW", Py_BuildValue("i", AUDIO_ENCODING_ALAW));
    PyModule_AddObject(m, "AUDIO_ENCODING_LINEAR", Py_BuildValue("i", AUDIO_ENCODING_LINEAR));

    PyModule_AddObject(m, "G726_16", Py_BuildValue("i", 16));
    PyModule_AddObject(m, "G726_24", Py_BuildValue("i", 24));
    PyModule_AddObject(m, "G726_32", Py_BuildValue("i", 32));
    PyModule_AddObject(m, "G726_40", Py_BuildValue("i", 40));

    return m;
}