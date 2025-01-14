//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2021 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include <any>
#include <map>
#include <pugixml-1.9/src/pugixml.hpp>

#include "config_error.h"
#include "item_name.h"
#include "value_type.h"

namespace cfg
{
	class DataSource
	{
	public:
		DataSource(const ov::String &current_path, const ov::String &file_name, const std::shared_ptr<pugi::xml_document> &document, const pugi::xml_node &node);
		DataSource(const ov::String &current_path, const ov::String &file_name, const ov::String json_name, const Json::Value &json);
		MAY_THROWS(std::shared_ptr<ConfigError>)
		DataSource(DataType type, const ov::String &current_path, const ov::String &file_name, const ItemName &root_name);

		DataType GetType() const
		{
			return _type;
		}

		const ov::String &ResolveName(const ItemName &name) const
		{
			return name.GetName(_type);
		}

		bool IsSourceOf(const ItemName &name) const
		{
			switch (_type)
			{
				case DataType::Xml:
					return name.xml_name == _node.name();

				case DataType::Json:
					return name.json_name == _json_name;
			}

			return false;
		}

		MAY_THROWS(std::shared_ptr<ConfigError>)
		void CheckUnknownItems(const ov::String &file_path, const ov::String &path, const std::map<ov::String, std::shared_ptr<Child>> &children_for_xml, const std::map<ov::String, std::shared_ptr<Child>> &children_for_json) const;

		// Check weather the root value is array or not
		bool IsArray(const ItemName &name) const;

		std::any GetRootValue(ValueType value_type, bool resolve_path, OmitRule omit_rule, Json::Value *original_value) const;
		std::any GetValue(ValueType value_type, const ItemName &name, bool resolve_path, OmitRule omit_rule, Json::Value *original_value) const;

		// Create a data source from this context
		DataSource NewDataSource(const ov::String &file_name, const ItemName &root_name) const
		{
			DataSource new_data_source(_type, _current_path, file_name, root_name);

			return new_data_source;
		}

		ov::String GetCurrentPath() const
		{
			return _current_path;
		}

		ov::String GetFileName() const
		{
			return _file_name;
		}

		ov::String ToString() const;

	protected:
		void LoadFromFile(ov::String file_name, const ItemName &root_name);

		void LoadFromXmlFile(const ov::String &file_name, const ov::String &root_name);
		void LoadFromJson(const ov::String &file_name, const ov::String &root_name);

		std::any GetValueFromXml(ValueType value_type, const ov::String &name, bool is_child, bool resolve_path, Json::Value *original_value) const;
		std::any GetValueFromJson(ValueType value_type, const ov::String &name, bool is_child, bool resolve_path, OmitRule omit_rule, Json::Value *original_value) const;

		DataType _type;

		std::shared_ptr<pugi::xml_document> _document;
		pugi::xml_node _node;

		ov::String _json_name;
		Json::Value _json;

		ov::String _current_path;
		ov::String _file_name;
	};
}  // namespace cfg
