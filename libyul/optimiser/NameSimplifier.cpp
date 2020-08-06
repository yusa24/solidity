/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libyul/optimiser/NameSimplifier.h>
#include <libyul/optimiser/NameCollector.h>
#include <libyul/AsmData.h>
#include <libdevcore/CommonData.h>
#include <regex>

using namespace yul;
using namespace std;

NameSimplifier::NameSimplifier(
	Block const& _ast,
	Dialect const& _dialect,
	set<YulString> _fixed
):
	m_dialect{_dialect},
	m_usedNames(std::move(_fixed))
{
	for (YulString name: _fixed)
		m_translations[name] = name;

	set<YulString> allNames = NameCollector(_ast).names();
	m_usedNames += allNames;
	for (YulString name: allNames)
		findSimplification(name);
}

void NameSimplifier::operator()(FunctionDefinition& _funDef)
{
	_funDef.name = m_translations.at(_funDef.name);
	renameVariables(_funDef.parameters);
	renameVariables(_funDef.returnVariables);
	ASTModifier::operator()(_funDef);
}

void NameSimplifier::operator()(VariableDeclaration& _varDecl)
{
	renameVariables(_varDecl.variables);
	ASTModifier::operator()(_varDecl);
}

void NameSimplifier::renameVariables(vector<TypedName>& _variables)
{
	for (TypedName& typedName: _variables)
		typedName.name = m_translations.at(typedName.name);
}

void NameSimplifier::operator()(Identifier& _identifier)
{
	_identifier.name = m_translations.at(_identifier.name);
}

void NameSimplifier::findSimplification(YulString _name)
{
	if (m_translations.count(_name))
		return;
	YulString newName = _name;
	// remove all IDs
	// remove `_to.*` for abi encoding
	// shorten string literal
	// t_contract$_NAME_$ -> _NAME_
		// _memory_ptr -> '_memory'
	// _memory -> ''

	// abi_encode_tuple_t_uint256_t_uint256_t_uint256__to_t_uint256_t_uint256_t_uint256__fromStack
	// array_allocation_size_t_array$_t_bytes_memory_$dyn_memory
	// abi_encode_tuple_t_uint256_t_uint256_t_address_payable_t_stringliteral_c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470__to_t_uint256_t_uint256_t_address_t_bytes_memory_ptr__fromStack
	// abi_encode_tuple_t_struct$_Order_$7595_memory_ptr__to_t_struct$_Order_$7595_memory_ptr__fromStack
	// abi_encode_tuple_t_stringliteral_d19f90e84893fa0d0f468bea4065b9ba26d437f65c7fc37321312b3982b97a83__to_t_bytes32__fromStack
	// abi_encode_tuple_t_contract$_IUniverse_$9337_t_contract$_IMarket_$8617_t_bytes32_t_bytes32_t_uint8_t_array$_t_address_$dyn_memory_ptr_t_array$_t_uint256_$dyn_memory_ptr__to_t_address_t_address_t_bytes32_t_bytes32_t_uint8_t_array$_t_address_$dyn_memory_ptr_t_array$_t_uint256_$dyn_memory_ptr__fromStack
	// abi_encode_tuple_t_contract$_ICash_$8316__to_t_address__fromStack
	// abi_decode_tuple_t_uint8t_uint256t_uint256t_addresst_uint8t_uint256t_uint256
	// abi_decode_t_bytes_calldata_ptr
	// abi_decode_tuple_t_uint256t_string_memory_ptrt_contract$_IMarket_$1504t_addresst_addresst_uint256t_array$_t_bytes32_$dyn_memory_ptr
	// abi_decode_tuple_t_contract$_IMarket_$1504
	// convert_t_contract$_IDisputeWindow_$1317_to_t_uint160
	// cleanup_t_enum$_MarketType_$1323
	// array_storeLengthForEncoding_t_string_memory_ptr_fromStack
	// array_storeLengthForEncoding_t_array$_t_bytes32_$dyn_memory_ptr_fromStack
	//
	// maybe try to shorten in stages as long as the name is still unique?
	m_translations[_name] = newName;
}
