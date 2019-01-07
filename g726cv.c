#include "Python.h"
#include "g72x.h"

typedef struct {
    PyObject_HEAD
    g726_state state_ptr;
} G726_cvObject;

static PyObject *g726_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    G726_cvObject *self;
    // we will free self in function g726_dealloc later
    self = (G726_cvObject *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static int g726_init(G726_cvObject *self, PyObject *args, PyObject *kwds) {
    g726_init_state(&(self->state_ptr));
    return 0;
}

static void g726_dealloc(G726_cvObject* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

/*
static PyMemberDef g726_members[] = {
    {NULL}
}*;/

/* pcm_to_g726_16() */
static PyObject *py_pcm_to_g726_16(G726_cvObject *self, PyObject *args) {
    char* buf = NULL;
    Py_ssize_t buf_len;

    // buf is managered by python, wen don't need to free
    int succeed = PyArg_ParseTuple(args, "y#", &buf, &buf_len);

    if(succeed){
        /* cast Py_ssize_t signed integer to unsigned */
        size_t size_t_MAX = -1;
        size_t unsigned_buf_len = buf_len + size_t_MAX + 1;

        // 16Bits -> 2Bit = 2^3
        size_t result_len = unsigned_buf_len >> 3;

        // we need to free it later
        char *buf2 = malloc(result_len);

        size_t index = 0;
        short* buf_short;

        for(index = 0; index < unsigned_buf_len; index+=8){
            buf_short = (short*)(buf + index);

            buf2[index >> 3] = (g726_16_encoder(buf_short[0], AUDIO_ENCODING_LINEAR, &self->state_ptr) << 6) |
            (g726_16_encoder(buf_short[1], AUDIO_ENCODING_LINEAR, &self->state_ptr) << 4) |
            (g726_16_encoder(buf_short[2], AUDIO_ENCODING_LINEAR, &self->state_ptr) << 2) |
            (g726_16_encoder(buf_short[3], AUDIO_ENCODING_LINEAR, &self->state_ptr) << 0);
        }

        PyObject *myResult = Py_BuildValue("y#", buf2, result_len);
        free(buf2);
        return myResult;
    }
    return NULL;
}

static PyMethodDef g726_methods[] = {
    {"pcm_to_g726_16", (PyCFunction)py_pcm_to_g726_16, METH_VARARGS, "Convert pcm 16bits to g276 2bits" },
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

    return m;
}