// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-26 04:43:47
// File  : python_wrapper.h
// Brief : https://docs.python.org/2/c-api/object.html
//         http://www.360doc.com/content/12/0506/13/9369336_209021809.shtml

#ifndef  __PYTHON_WRAPPER_H_
#define  __PYTHON_WRAPPER_H_

#include "third_part/python/include/python2.7/Python.h"
#include "base/public/logging.h"
#include "file/public/file.h"

namespace python_wrapper {

// muset be only-one instance at any time (obvious not-threadsafe).
class PythonWrapper {
 public:
  PythonWrapper(const std::string & file_path) : file_path_(file_path) {
    Py_Initialize();
    std::string path = Py_GetPath();
    std::string cur = file::File::DirName(file_path_);
    if (cur.size() > 0) {
      path = cur + ":" + path;
    }
    PySys_SetPath((char *)path.c_str());

    std::string name = file::File::BaseName(file_path_);
    if (name.find('.') != std::string::npos) {
      name = name.substr(0, name.find('.'));
    }

    module_ = PyImport_ImportModule(name.c_str());
    if (moduler_ == NULL) PyErr_Print();
    CHECK(module_ != NULL)
      << "import " << file_path_ << " name=" << name << " error";
  }
  
  void CallFunc(const std::string & name,
                const std::vector<std::string> argc,
                std::string * output = NULL) {
    PyObject * input = NULL;
    for (int i = 0; i < argc.size(); i++) {
      if (input == NULL) {
        input = PyTuple_New(argc.size());
      }
      PyTuple_SetItem(input, i, Py_BuildValue("s", argc[i].c_str()));
    }
  
    PyObject * func = PyObject_GetAttrString(module_, name.c_str());
    CHECK(func != NULL) << "not find func " << name;
    PyObject * rs = PyEval_CallObject(func, input);
    CHECK(rs != NULL) << "run " << name << " error";
    if (output) {
      output->clear();
      char * tmp = NULL;
      PyArg_Parse(rs, "s", &tmp);
      output->append(tmp);
    }
  }

  void CallClassFunc(const std::string & class_name,
                     const std::string & func_name,
                     const std::vector<std::string> & class_argc,
                     std::string * output = NULL) {
    PyObject * class_input = NULL;
    for (int i = 0; i < class_argc.size(); i++) {
      if (class_input == NULL) {
        class_input = PyTuple_New(class_argc.size());
      }
      PyTuple_SetItem(class_input, i,
                      Py_BuildValue("s", class_argc[i].c_str()));
    }

    PyObject * class_ptr = PyObject_GetAttrString(module_, class_name.c_str());
    CHECK(class_ptr != NULL) << "get " << class_name << " error.";
    PyObject * object = PyInstance_New(class_ptr, class_input, NULL);
    CHECK(object != NULL) << "instance new error.";

    PyObject * name = PyString_FromString(func_name.c_str());
    PyObject * rs = PyObject_CallMethodObjArgs(object, name, NULL, NULL);
    CHECK(rs != NULL) << "run " << func_name << " error";
    if (output) {
      output->clear();
      char * tmp = NULL;
      PyArg_Parse(rs, "s", &tmp);
      output->append(tmp);
    }
  }

  void CallClassFunc(const std::string & class_name,
                     const std::string & func_name,
                     const std::vector<std::string> & class_argc,
                     const std::string & func_argc,
                     std::string * output = NULL) {
    PyObject * class_input = NULL;
    for (int i = 0; i < class_argc.size(); i++) {
      if (class_input == NULL) {
        class_input = PyTuple_New(class_argc.size());
      }
      PyTuple_SetItem(class_input, i,
                      Py_BuildValue("s", class_argc[i].c_str()));
    }

    PyObject * func_input = Py_BuildValue("s", func_argc.c_str());

    PyObject * class_ptr = PyObject_GetAttrString(module_, class_name.c_str());
    CHECK(class_ptr != NULL) << "get " << class_name << " error.";
    PyObject * object = PyInstance_New(class_ptr, class_input, NULL);
    CHECK(object != NULL) << "instance new error.";

    PyObject * name = PyString_FromString(func_name.c_str());
    PyObject * rs = PyObject_CallMethodObjArgs(object, name, func_input, NULL);
    CHECK(rs != NULL) << "run " << func_name << " error";
    if (output) {
      output->clear();
      char * tmp = NULL;
      PyArg_Parse(rs, "s", &tmp);
      output->append(tmp);
    }
  }

  void CallClassFunc(const std::string & class_name,
                     const std::string & func_name,
                     const std::vector<std::string> & class_argc,
                     const std::string & func_argc1,
                     const std::string & func_argc2,
                     std::string * output = NULL) {
    PyObject * class_input = NULL;
    for (int i = 0; i < class_argc.size(); i++) {
      if (class_input == NULL) {
        class_input = PyTuple_New(class_argc.size());
      }
      PyTuple_SetItem(class_input, i,
                      Py_BuildValue("s", class_argc[i].c_str()));
    }

    PyObject * func_input1 = Py_BuildValue("s", func_argc1.c_str());
    PyObject * func_input2 = Py_BuildValue("s", func_argc2.c_str());

    PyObject * class_ptr = PyObject_GetAttrString(module_, class_name.c_str());
    CHECK(class_ptr != NULL) << "get " << class_name << " error.";
    PyObject * object = PyInstance_New(class_ptr, class_input, NULL);
    CHECK(object != NULL) << "instance new error.";

    PyObject * name = PyString_FromString(func_name.c_str());
    PyObject * rs = PyObject_CallMethodObjArgs(object,
                                               name,
                                               func_input1,
                                               func_input2,
                                               NULL);
    CHECK(rs != NULL) << "run " << func_name << " error";
    if (output) {
      output->clear();
      char * tmp = NULL;
      PyArg_Parse(rs, "s", &tmp);
      output->append(tmp);
    }
  }

  void CallClassFunc(const std::string & class_name,
                     const std::string & func_name,
                     const std::vector<std::string> & class_argc,
                     const std::string & func_argc1,
                     const std::string & func_argc2,
                     const std::string & func_argc3,
                     std::string * output = NULL) {
    PyObject * class_input = NULL;
    for (int i = 0; i < class_argc.size(); i++) {
      if (class_input == NULL) {
        class_input = PyTuple_New(class_argc.size());
      }
      PyTuple_SetItem(class_input, i,
                      Py_BuildValue("s", class_argc[i].c_str()));
    }

    PyObject * func_input1 = Py_BuildValue("s", func_argc1.c_str());
    PyObject * func_input2 = Py_BuildValue("s", func_argc2.c_str());
    PyObject * func_input3 = Py_BuildValue("s", func_argc3.c_str());

    PyObject * class_ptr = PyObject_GetAttrString(module_, class_name.c_str());
    CHECK(class_ptr != NULL) << "get " << class_name << " error.";
    PyObject * object = PyInstance_New(class_ptr, class_input, NULL);
    CHECK(object != NULL) << "instance new error.";

    PyObject * name = PyString_FromString(func_name.c_str());
    PyObject * rs = PyObject_CallMethodObjArgs(object,
                                               name,
                                               func_input1,
                                               func_input2,
                                               func_input3,
                                               NULL);
    CHECK(rs != NULL) << "run " << func_name << " error";
    if (output) {
      output->clear();
      char * tmp = NULL;
      PyArg_Parse(rs, "s", &tmp);
      output->append(tmp);
    }
  }

  void CallClassFunc(const std::string & class_name,
                     const std::string & func_name,
                     const std::vector<std::string> & class_argc,
                     const std::string & func_argc1,
                     const std::string & func_argc2,
                     const std::string & func_argc3,
                     const std::string & func_argc4,
                     std::string * output = NULL) {
    PyObject * class_input = NULL;
    for (int i = 0; i < class_argc.size(); i++) {
      if (class_input == NULL) {
        class_input = PyTuple_New(class_argc.size());
      }
      PyTuple_SetItem(class_input, i,
                      Py_BuildValue("s", class_argc[i].c_str()));
    }

    PyObject * func_input1 = Py_BuildValue("s", func_argc1.c_str());
    PyObject * func_input2 = Py_BuildValue("s", func_argc2.c_str());
    PyObject * func_input3 = Py_BuildValue("s", func_argc3.c_str());
    PyObject * func_input4 = Py_BuildValue("s", func_argc4.c_str());

    PyObject * class_ptr = PyObject_GetAttrString(module_, class_name.c_str());
    CHECK(class_ptr != NULL) << "get " << class_name << " error.";
    PyObject * object = PyInstance_New(class_ptr, class_input, NULL);
    CHECK(object != NULL) << "instance new error.";

    PyObject * name = PyString_FromString(func_name.c_str());
    PyObject * rs = PyObject_CallMethodObjArgs(object,
                                               name,
                                               func_input1,
                                               func_input2,
                                               func_input3,
                                               func_input4,
                                               NULL);
    CHECK(rs != NULL) << "run " << func_name << " error";
    if (output) {
      output->clear();
      char * tmp = NULL;
      PyArg_Parse(rs, "s", &tmp);
      output->append(tmp);
    }
  }

  ~PythonWrapper() {
    Py_Finalize();
  }

 private:
  std::string file_path_;
  PyObject * module_;
};

}  // namespace python_wrapper

#endif  //__PYTHON_WRAPPER_H_
