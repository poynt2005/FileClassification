#include <Python.h>
#include <vector>
#include <string>
#include "../FileClassificator.h"

static PyObject *PyClassificate(PyObject *self, PyObject *args)
{
    PyObject *pyBuiltinsModule = PyImport_ImportModule("builtins");
    PyObject *pyCallableChecker = PyObject_GetAttrString(pyBuiltinsModule, "callable");

    auto callableChecker = [&pyCallableChecker](PyObject *pyObjectIn) -> bool
    {
        PyObject *pyCallableCheckResult = PyObject_CallFunction(pyCallableChecker, "O", pyObjectIn);
        bool pyTrueCheckResult = Py_IsTrue(pyCallableCheckResult);
        Py_DECREF(pyCallableCheckResult);
        return pyTrueCheckResult;
    };

    const char *fileSrcDirPath;
    const char *fileDstDirPath;
    PyObject *pyProgresser = NULL;

    if (!PyArg_ParseTuple(args, "ss|O", &fileSrcDirPath, &fileDstDirPath, &pyProgresser))
    {
        return NULL;
    }

    PyObject *pyMovedFilesList = PyList_New(0);
    std::vector<std::string> movedFilesList;
    if (pyProgresser != NULL && callableChecker(pyProgresser))
    {

        Classificator::Classificate(std::string(fileSrcDirPath), std::string(fileDstDirPath), movedFilesList, [&pyProgresser](const Classificator::ProgressInfo info, const double ratio, const std::string &fileCalculateInfo) -> void
                                    {
                                        std::string progressInfoText;
                                        Classificator::GetProgressInfoText(info, progressInfoText);

                                        PyObject *pyUtf8Info = PyUnicode_Decode(progressInfoText.data(), progressInfoText.length(), "utf-8", "ignore");
                                        PyObject *pyUtf8CalInfo = PyUnicode_Decode(fileCalculateInfo.data(), fileCalculateInfo.length(), "utf-8", "ignore");
                                        PyObject *pyCallFnResult = PyObject_CallFunction(pyProgresser, "OdO", pyUtf8Info, ratio, pyUtf8CalInfo);
                                        Py_DECREF(pyCallFnResult);
                                        Py_DECREF(pyUtf8CalInfo);
                                        Py_DECREF(pyUtf8Info); });
    }
    else
    {
        Classificator::Classificate(std::string(fileSrcDirPath), std::string(fileDstDirPath), movedFilesList);
    }

    for (int i = 0; i < movedFilesList.size(); i++)
    {
        auto entry = std::move(movedFilesList[i]);
        PyObject *pyFileStr = PyUnicode_FromString(entry.data());

        PyList_Append(pyMovedFilesList, pyFileStr);
        Py_DECREF(pyFileStr);
    }

    Py_DECREF(pyCallableChecker);
    Py_DECREF(pyBuiltinsModule);
    return pyMovedFilesList;
}

static PyObject *PyGetSHA512StringValue(PyObject *self, PyObject *args)
{
    const char *filePath;

    if (!PyArg_ParseTuple(args, "s", &filePath))
    {
        return NULL;
    }

    std::string fileHash;
    Hasher::GetSHA512StringValue(std::string(filePath), fileHash);

    PyObject *pyResultHashStr = PyUnicode_FromString(fileHash.data());

    return pyResultHashStr;
}

static PyMethodDef methods[] = {
    {"Classificate", PyClassificate, METH_VARARGS, NULL},
    {"GetSHA512StringValue", PyGetSHA512StringValue, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "PyFileClassificator",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC
PyInit_PyFileClassificator(void)
{
    return PyModule_Create(&module);
}
