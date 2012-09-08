/*
 * Appcelerator Kroll - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _STATIC_BOUND_METHOD_H_
#define _STATIC_BOUND_METHOD_H_

namespace tide
{

	class TIDE_API StaticBoundMethod : public Method
	{
	public:

		StaticBoundMethod(MethodCallback* callback, const char *type = "StaticBoundMethod");
		virtual ~StaticBoundMethod();

		/**
		 * @see Method::Call
		 */
		virtual ValueRef Call(const ValueList& args);

		/**
		 * @see Object::Set
		 */
		virtual void Set(const char *name, ValueRef value);

		/**
		 * @see Object::Get
		 */
		virtual ValueRef Get(const char *name);

		/**
		 * @see Object::GetPropertyNames
		 */
		virtual SharedStringList GetPropertyNames();
		
		/**
		 * Set a property on this object to the given method. When an error
		 * occurs will throw an exception of type ValueException.
		 */
		template <typename T>
		void SetMethod(const char *name, void (T::*method)(const ValueList&, ValueRef))
		{
			MethodCallback* callback = NewCallback<T, const ValueList&, ValueRef>(static_cast<T*>(this), method);

			MethodRef bound_method = new StaticBoundMethod(callback);
			ValueRef method_value = Value::NewMethod(bound_method);
			this->Set(name, method_value);
		}

		template <typename T>
		static AutoPtr<StaticBoundMethod> FromMethod(T* owner, void (T::*method)(const ValueList&, ValueRef))
		{
			MethodCallback* callback = NewCallback<T, const ValueList&, ValueRef>(static_cast<T*>(owner), method);
			return new StaticBoundMethod(callback);
		}

	protected:
		SharedPtr<MethodCallback> callback;
		AutoPtr<StaticBoundObject> object;
		std::map<std::string, ValueRef > properties;

	private:
		DISALLOW_EVIL_CONSTRUCTORS(StaticBoundMethod);
	};
}

#endif