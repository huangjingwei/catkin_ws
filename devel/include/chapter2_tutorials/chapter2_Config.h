//#line 2 "/opt/ros/indigo/share/dynamic_reconfigure/cmake/../templates/ConfigType.h.template"
// *********************************************************
//
// File autogenerated for the chapter2_tutorials package
// by the dynamic_reconfigure package.
// Please do not edit.
//
// ********************************************************/

#ifndef __chapter2_tutorials__CHAPTER2_CONFIG_H__
#define __chapter2_tutorials__CHAPTER2_CONFIG_H__

#include <dynamic_reconfigure/config_tools.h>
#include <limits>
#include <ros/node_handle.h>
#include <dynamic_reconfigure/ConfigDescription.h>
#include <dynamic_reconfigure/ParamDescription.h>
#include <dynamic_reconfigure/Group.h>
#include <dynamic_reconfigure/config_init_mutex.h>
#include <boost/any.hpp>

namespace chapter2_tutorials
{
  class chapter2_ConfigStatics;

  class chapter2_Config
  {
  public:
    class AbstractParamDescription : public dynamic_reconfigure::ParamDescription
    {
    public:
      AbstractParamDescription(std::string n, std::string t, uint32_t l,
          std::string d, std::string e)
      {
        name = n;
        type = t;
        level = l;
        description = d;
        edit_method = e;
      }

      virtual void clamp(chapter2_Config &config, const chapter2_Config &max, const chapter2_Config &min) const = 0;
      virtual void calcLevel(uint32_t &level, const chapter2_Config &config1, const chapter2_Config &config2) const = 0;
      virtual void fromServer(const ros::NodeHandle &nh, chapter2_Config &config) const = 0;
      virtual void toServer(const ros::NodeHandle &nh, const chapter2_Config &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, chapter2_Config &config) const = 0;
      virtual void toMessage(dynamic_reconfigure::Config &msg, const chapter2_Config &config) const = 0;
      virtual void getValue(const chapter2_Config &config, boost::any &val) const = 0;
    };

    typedef boost::shared_ptr<AbstractParamDescription> AbstractParamDescriptionPtr;
    typedef boost::shared_ptr<const AbstractParamDescription> AbstractParamDescriptionConstPtr;

    template <class T>
    class ParamDescription : public AbstractParamDescription
    {
    public:
      ParamDescription(std::string a_name, std::string a_type, uint32_t a_level,
          std::string a_description, std::string a_edit_method, T chapter2_Config::* a_f) :
        AbstractParamDescription(a_name, a_type, a_level, a_description, a_edit_method),
        field(a_f)
      {}

      T (chapter2_Config::* field);

      virtual void clamp(chapter2_Config &config, const chapter2_Config &max, const chapter2_Config &min) const
      {
        if (config.*field > max.*field)
          config.*field = max.*field;

        if (config.*field < min.*field)
          config.*field = min.*field;
      }

      virtual void calcLevel(uint32_t &comb_level, const chapter2_Config &config1, const chapter2_Config &config2) const
      {
        if (config1.*field != config2.*field)
          comb_level |= level;
      }

      virtual void fromServer(const ros::NodeHandle &nh, chapter2_Config &config) const
      {
        nh.getParam(name, config.*field);
      }

      virtual void toServer(const ros::NodeHandle &nh, const chapter2_Config &config) const
      {
        nh.setParam(name, config.*field);
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, chapter2_Config &config) const
      {
        return dynamic_reconfigure::ConfigTools::getParameter(msg, name, config.*field);
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const chapter2_Config &config) const
      {
        dynamic_reconfigure::ConfigTools::appendParameter(msg, name, config.*field);
      }

      virtual void getValue(const chapter2_Config &config, boost::any &val) const
      {
        val = config.*field;
      }
    };

    class AbstractGroupDescription : public dynamic_reconfigure::Group
    {
      public:
      AbstractGroupDescription(std::string n, std::string t, int p, int i, bool s)
      {
        name = n;
        type = t;
        parent = p;
        state = s;
        id = i;
      }

      std::vector<AbstractParamDescriptionConstPtr> abstract_parameters;
      bool state;

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &config) const =0;
      virtual void updateParams(boost::any &cfg, chapter2_Config &top) const= 0;
      virtual void setInitialState(boost::any &cfg) const = 0;


      void convertParams()
      {
        for(std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = abstract_parameters.begin(); i != abstract_parameters.end(); ++i)
        {
          parameters.push_back(dynamic_reconfigure::ParamDescription(**i));
        }
      }
    };

    typedef boost::shared_ptr<AbstractGroupDescription> AbstractGroupDescriptionPtr;
    typedef boost::shared_ptr<const AbstractGroupDescription> AbstractGroupDescriptionConstPtr;

    template<class T, class PT>
    class GroupDescription : public AbstractGroupDescription
    {
    public:
      GroupDescription(std::string a_name, std::string a_type, int a_parent, int a_id, bool a_s, T PT::* a_f) : AbstractGroupDescription(a_name, a_type, a_parent, a_id, a_s), field(a_f)
      {
      }

      GroupDescription(const GroupDescription<T, PT>& g): AbstractGroupDescription(g.name, g.type, g.parent, g.id, g.state), field(g.field), groups(g.groups)
      {
        parameters = g.parameters;
        abstract_parameters = g.abstract_parameters;
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &cfg) const
      {
        PT* config = boost::any_cast<PT*>(cfg);
        if(!dynamic_reconfigure::ConfigTools::getGroupState(msg, name, (*config).*field))
          return false;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          if(!(*i)->fromMessage(msg, n))
            return false;
        }

        return true;
      }

      virtual void setInitialState(boost::any &cfg) const
      {
        PT* config = boost::any_cast<PT*>(cfg);
        T* group = &((*config).*field);
        group->state = state;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = boost::any(&((*config).*field));
          (*i)->setInitialState(n);
        }

      }

      virtual void updateParams(boost::any &cfg, chapter2_Config &top) const
      {
        PT* config = boost::any_cast<PT*>(cfg);

        T* f = &((*config).*field);
        f->setParams(top, abstract_parameters);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          (*i)->updateParams(n, top);
        }
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &cfg) const
      {
        const PT config = boost::any_cast<PT>(cfg);
        dynamic_reconfigure::ConfigTools::appendGroup<T>(msg, name, id, parent, config.*field);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          (*i)->toMessage(msg, config.*field);
        }
      }

      T (PT::* field);
      std::vector<chapter2_Config::AbstractGroupDescriptionConstPtr> groups;
    };

class DEFAULT
{
  public:
    DEFAULT()
    {
      state = true;
      name = "Default";
    }

    void setParams(chapter2_Config &config, const std::vector<AbstractParamDescriptionConstPtr> params)
    {
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator _i = params.begin(); _i != params.end(); ++_i)
      {
        boost::any val;
        (*_i)->getValue(config, val);

        if("int_param"==(*_i)->name){int_param = boost::any_cast<int>(val);}
        if("double_param"==(*_i)->name){double_param = boost::any_cast<double>(val);}
        if("str_param"==(*_i)->name){str_param = boost::any_cast<std::string>(val);}
        if("bool_param"==(*_i)->name){bool_param = boost::any_cast<bool>(val);}
        if("size"==(*_i)->name){size = boost::any_cast<int>(val);}
      }
    }

    int int_param;
double double_param;
std::string str_param;
bool bool_param;
int size;

    bool state;
    std::string name;

    
}groups;



//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      int int_param;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      double double_param;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      std::string str_param;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      bool bool_param;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      int size;
//#line 218 "/opt/ros/indigo/share/dynamic_reconfigure/cmake/../templates/ConfigType.h.template"

    bool __fromMessage__(dynamic_reconfigure::Config &msg)
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();

      int count = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        if ((*i)->fromMessage(msg, *this))
          count++;

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i ++)
      {
        if ((*i)->id == 0)
        {
          boost::any n = boost::any(this);
          (*i)->updateParams(n, *this);
          (*i)->fromMessage(msg, n);
        }
      }

      if (count != dynamic_reconfigure::ConfigTools::size(msg))
      {
        ROS_ERROR("chapter2_Config::__fromMessage__ called with an unexpected parameter.");
        ROS_ERROR("Booleans:");
        for (unsigned int i = 0; i < msg.bools.size(); i++)
          ROS_ERROR("  %s", msg.bools[i].name.c_str());
        ROS_ERROR("Integers:");
        for (unsigned int i = 0; i < msg.ints.size(); i++)
          ROS_ERROR("  %s", msg.ints[i].name.c_str());
        ROS_ERROR("Doubles:");
        for (unsigned int i = 0; i < msg.doubles.size(); i++)
          ROS_ERROR("  %s", msg.doubles[i].name.c_str());
        ROS_ERROR("Strings:");
        for (unsigned int i = 0; i < msg.strs.size(); i++)
          ROS_ERROR("  %s", msg.strs[i].name.c_str());
        // @todo Check that there are no duplicates. Make this error more
        // explicit.
        return false;
      }
      return true;
    }

    // This version of __toMessage__ is used during initialization of
    // statics when __getParamDescriptions__ can't be called yet.
    void __toMessage__(dynamic_reconfigure::Config &msg, const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__, const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__) const
    {
      dynamic_reconfigure::ConfigTools::clear(msg);
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toMessage(msg, *this);

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        if((*i)->id == 0)
        {
          (*i)->toMessage(msg, *this);
        }
      }
    }

    void __toMessage__(dynamic_reconfigure::Config &msg) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      __toMessage__(msg, __param_descriptions__, __group_descriptions__);
    }

    void __toServer__(const ros::NodeHandle &nh) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toServer(nh, *this);
    }

    void __fromServer__(const ros::NodeHandle &nh)
    {
      static bool setup=false;

      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->fromServer(nh, *this);

      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i++){
        if (!setup && (*i)->id == 0) {
          setup = true;
          boost::any n = boost::any(this);
          (*i)->setInitialState(n);
        }
      }
    }

    void __clamp__()
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const chapter2_Config &__max__ = __getMax__();
      const chapter2_Config &__min__ = __getMin__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->clamp(*this, __max__, __min__);
    }

    uint32_t __level__(const chapter2_Config &config) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      uint32_t level = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->calcLevel(level, config, *this);
      return level;
    }

    static const dynamic_reconfigure::ConfigDescription &__getDescriptionMessage__();
    static const chapter2_Config &__getDefault__();
    static const chapter2_Config &__getMax__();
    static const chapter2_Config &__getMin__();
    static const std::vector<AbstractParamDescriptionConstPtr> &__getParamDescriptions__();
    static const std::vector<AbstractGroupDescriptionConstPtr> &__getGroupDescriptions__();

  private:
    static const chapter2_ConfigStatics *__get_statics__();
  };

  template <> // Max and min are ignored for strings.
  inline void chapter2_Config::ParamDescription<std::string>::clamp(chapter2_Config &config, const chapter2_Config &max, const chapter2_Config &min) const
  {
    (void) config;
    (void) min;
    (void) max;
    return;
  }

  class chapter2_ConfigStatics
  {
    friend class chapter2_Config;

    chapter2_ConfigStatics()
    {
chapter2_Config::GroupDescription<chapter2_Config::DEFAULT, chapter2_Config> Default("Default", "", 0, 0, true, &chapter2_Config::groups);
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.int_param = 0;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.int_param = 100;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.int_param = 1;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<int>("int_param", "int", 0, "An Integer parameter", "", &chapter2_Config::int_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<int>("int_param", "int", 0, "An Integer parameter", "", &chapter2_Config::int_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.double_param = 0.0;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.double_param = 1.0;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.double_param = 0.1;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<double>("double_param", "double", 0, "A double parameter", "", &chapter2_Config::double_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<double>("double_param", "double", 0, "A double parameter", "", &chapter2_Config::double_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.str_param = "";
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.str_param = "";
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.str_param = "Chapter2_dynamic_reconfigure";
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<std::string>("str_param", "str", 0, "A string parameter", "", &chapter2_Config::str_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<std::string>("str_param", "str", 0, "A string parameter", "", &chapter2_Config::str_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.bool_param = 0;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.bool_param = 1;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.bool_param = 1;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<bool>("bool_param", "bool", 0, "A Boolean parameter", "", &chapter2_Config::bool_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<bool>("bool_param", "bool", 0, "A Boolean parameter", "", &chapter2_Config::bool_param)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.size = 0;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.size = 3;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.size = 1;
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<int>("size", "int", 0, "Select from the list", "{'enum_description': 'Select from the list', 'enum': [{'srcline': 13, 'description': 'Low is 0', 'srcfile': '/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg', 'cconsttype': 'const int', 'value': 0, 'ctype': 'int', 'type': 'int', 'name': 'Low'}, {'srcline': 14, 'description': 'Medium is 1', 'srcfile': '/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'Medium'}, {'srcline': 15, 'description': 'Hight is 2', 'srcfile': '/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg', 'cconsttype': 'const int', 'value': 2, 'ctype': 'int', 'type': 'int', 'name': 'High'}]}", &chapter2_Config::size)));
//#line 292 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(chapter2_Config::AbstractParamDescriptionConstPtr(new chapter2_Config::ParamDescription<int>("size", "int", 0, "Select from the list", "{'enum_description': 'Select from the list', 'enum': [{'srcline': 13, 'description': 'Low is 0', 'srcfile': '/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg', 'cconsttype': 'const int', 'value': 0, 'ctype': 'int', 'type': 'int', 'name': 'Low'}, {'srcline': 14, 'description': 'Medium is 1', 'srcfile': '/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg', 'cconsttype': 'const int', 'value': 1, 'ctype': 'int', 'type': 'int', 'name': 'Medium'}, {'srcline': 15, 'description': 'Hight is 2', 'srcfile': '/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg', 'cconsttype': 'const int', 'value': 2, 'ctype': 'int', 'type': 'int', 'name': 'High'}]}", &chapter2_Config::size)));
//#line 245 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.convertParams();
//#line 245 "/opt/ros/indigo/lib/python2.7/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __group_descriptions__.push_back(chapter2_Config::AbstractGroupDescriptionConstPtr(new chapter2_Config::GroupDescription<chapter2_Config::DEFAULT, chapter2_Config>(Default)));
//#line 356 "/opt/ros/indigo/share/dynamic_reconfigure/cmake/../templates/ConfigType.h.template"

      for (std::vector<chapter2_Config::AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        __description_message__.groups.push_back(**i);
      }
      __max__.__toMessage__(__description_message__.max, __param_descriptions__, __group_descriptions__);
      __min__.__toMessage__(__description_message__.min, __param_descriptions__, __group_descriptions__);
      __default__.__toMessage__(__description_message__.dflt, __param_descriptions__, __group_descriptions__);
    }
    std::vector<chapter2_Config::AbstractParamDescriptionConstPtr> __param_descriptions__;
    std::vector<chapter2_Config::AbstractGroupDescriptionConstPtr> __group_descriptions__;
    chapter2_Config __max__;
    chapter2_Config __min__;
    chapter2_Config __default__;
    dynamic_reconfigure::ConfigDescription __description_message__;

    static const chapter2_ConfigStatics *get_instance()
    {
      // Split this off in a separate function because I know that
      // instance will get initialized the first time get_instance is
      // called, and I am guaranteeing that get_instance gets called at
      // most once.
      static chapter2_ConfigStatics instance;
      return &instance;
    }
  };

  inline const dynamic_reconfigure::ConfigDescription &chapter2_Config::__getDescriptionMessage__()
  {
    return __get_statics__()->__description_message__;
  }

  inline const chapter2_Config &chapter2_Config::__getDefault__()
  {
    return __get_statics__()->__default__;
  }

  inline const chapter2_Config &chapter2_Config::__getMax__()
  {
    return __get_statics__()->__max__;
  }

  inline const chapter2_Config &chapter2_Config::__getMin__()
  {
    return __get_statics__()->__min__;
  }

  inline const std::vector<chapter2_Config::AbstractParamDescriptionConstPtr> &chapter2_Config::__getParamDescriptions__()
  {
    return __get_statics__()->__param_descriptions__;
  }

  inline const std::vector<chapter2_Config::AbstractGroupDescriptionConstPtr> &chapter2_Config::__getGroupDescriptions__()
  {
    return __get_statics__()->__group_descriptions__;
  }

  inline const chapter2_ConfigStatics *chapter2_Config::__get_statics__()
  {
    const static chapter2_ConfigStatics *statics;

    if (statics) // Common case
      return statics;

    boost::mutex::scoped_lock lock(dynamic_reconfigure::__init_mutex__);

    if (statics) // In case we lost a race.
      return statics;

    statics = chapter2_ConfigStatics::get_instance();

    return statics;
  }

//#line 13 "/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg"
      const int chapter2__Low = 0;
//#line 14 "/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg"
      const int chapter2__Medium = 1;
//#line 15 "/home/catkin_ws/src/chapter2_tutorials/cfg/chapter2.cfg"
      const int chapter2__High = 2;
}

#endif // __CHAPTER2_RECONFIGURATOR_H__
