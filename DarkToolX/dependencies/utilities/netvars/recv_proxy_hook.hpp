#pragma once
#include "../../../source-sdk/classes/recv_props.hpp"

class recv_prop_hook
{
public:
	constexpr recv_prop_hook(recv_prop* prop, const recv_var_proxy_fn proxy_fn) :
		prop(prop),
		original_proxy_fn(prop->proxy_fn)
	{
		this->prop->proxy_fn = proxy_fn;
	}
	~recv_prop_hook()
	{
		prop->proxy_fn = original_proxy_fn;
	}
	constexpr recv_var_proxy_fn get_original_function() const
	{
		return original_proxy_fn;
	}
private:
	recv_prop* prop;
	recv_var_proxy_fn original_proxy_fn;
};
