// Protocol Buffers for Objective C
//
// Copyright 2010 Booyah Inc.
// Copyright 2008 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "objc_enum_field.h"

#include <map>
#include <string>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/stubs/strutil.h>

#include "objc_helpers.h"

namespace google { namespace protobuf { namespace compiler { namespace objectivec {

  namespace {
    void SetEnumVariables(const FieldDescriptor* descriptor,
      map<string, string>* variables) {
        const EnumValueDescriptor* default_value;
        default_value = descriptor->default_value_enum();

        string type = ClassName(descriptor->enum_type());

        (*variables)["classname"]             = ClassName(descriptor->containing_type());
        (*variables)["name"]                  = UnderscoresToCamelCase(descriptor);
        (*variables)["capitalized_name"]      = UnderscoresToCapitalizedCamelCase(descriptor);
        (*variables)["list_name"]             = UnderscoresToCamelCase(descriptor) + "Array";
        (*variables)["number"] = SimpleItoa(descriptor->number());
        (*variables)["type"] = type;
        (*variables)["default"] = EnumValueName(default_value);
        (*variables)["tag"] = SimpleItoa(internal::WireFormat::MakeTag(descriptor));
        (*variables)["tag_size"] = SimpleItoa(
          internal::WireFormat::TagSize(descriptor->number(), descriptor->type()));
    }
  }  // namespace

  EnumFieldGenerator::EnumFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor) {
      SetEnumVariables(descriptor, &variables_);
  }


  EnumFieldGenerator::~EnumFieldGenerator() {
  }


  void EnumFieldGenerator::GenerateHasFieldHeader(io::Printer* printer) const {
    printer->Print(variables_, "BOOL has$capitalized_name$_:1;\n");
  }


  void EnumFieldGenerator::GenerateFieldHeader(io::Printer* printer) const {
    printer->Print(variables_, "$type$ $name$;\n");
  }


  void EnumFieldGenerator::GenerateHasPropertyHeader(io::Printer* printer) const {
    printer->Print(variables_,"- (BOOL)has$capitalized_name$;\n");
  }


  void EnumFieldGenerator::GeneratePropertyHeader(io::Printer* printer) const {
    printer->Print(variables_,
      "@property (nonatomic, readonly) $type$ $name$;\n");
  }


  void EnumFieldGenerator::GenerateExtensionSource(io::Printer* printer) const {
    printer->Print(variables_,"@property (nonatomic, readwrite) BOOL has$capitalized_name$;\n");
    printer->Print(variables_,"@property (nonatomic, readwrite) $type$ $name$;\n");
  }


  void EnumFieldGenerator::GenerateMembersHeader(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateMembersSource(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateSynthesizeSource(io::Printer* printer) const {
  }

  void EnumFieldGenerator::GenerateInitializationSource(io::Printer* printer) const {
    printer->Print(variables_, "self.$name$ = $default$;\n");
  }


  void EnumFieldGenerator::GenerateBuilderMembersHeader(io::Printer* printer) const {
    printer->Print(variables_,"- ($classname$_Builder*)set$capitalized_name$:($type$) value;\n");
  }

  void EnumFieldGenerator::GenerateBuilderGetterHeader(io::Printer* printer) const {
    printer->Print(variables_,
      "- ($type$)$name$;\n"
      "- (BOOL)has$capitalized_name$;\n");
  }

  void EnumFieldGenerator::GenerateBuilderClearHeader(io::Printer* printer) const {
    printer->Print(variables_,"- ($classname$_Builder*)clear$capitalized_name$;\n");
  }

  void EnumFieldGenerator::GenerateBuilderGetterSource(io::Printer* printer) const {
    printer->Print(variables_,
          "- ($type$)$name$ {\n"
          "  return builder_result.$name$;\n"
          "}\n");
    printer->Print(variables_,
      "- (BOOL)has$capitalized_name$ {\n"
      " return builder_result.has$capitalized_name$;\n"
      "}\n");
  }

  void EnumFieldGenerator::GenerateBuilderMembersSource(io::Printer* printer) const {
    printer->Print(variables_,
      "- ($classname$_Builder*)set$capitalized_name$:($type$) value {\n"
      "  NSAssert($type$IsValidValue(value), @\"The value '%d' is invalid for $type$\", value);\n"
      "  builder_result.has$capitalized_name$ = YES;\n"
      "  builder_result.$name$ = value;\n"
      "  return self;\n"
      "}\n");
  }

  void EnumFieldGenerator::GenerateBuilderClearSource(io::Printer* printer) const {
    printer->Print(variables_,
          "- ($classname$_Builder*)clear$capitalized_name$ {\n"
          "  builder_result.has$capitalized_name$ = NO;\n"
          "  builder_result.$name$ = $default$;\n"
          "  return self;\n"
          "}\n");
  }

  void EnumFieldGenerator::GenerateMergingCodeHeader(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateMergingCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "if (other.has$capitalized_name$) {\n"
      "  [self set$capitalized_name$:other.$name$];\n"
      "}\n");
  }

  void EnumFieldGenerator::GenerateBuildingCodeHeader(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateBuildingCodeSource(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateParsingCodeHeader(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateParsingCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "$type$ value = ($type$)[input readEnum];\n"
      "if ($type$IsValidValue(value)) {\n"
      "  [self set$capitalized_name$:value];\n"
      "} else {\n"
      "  [unknownFields mergeVarintField:$number$ value:value];\n"
      "}\n");
  }


  void EnumFieldGenerator::GenerateSerializationCodeHeader(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateSerializationCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "if (self.has$capitalized_name$) {\n"
      "  [output writeEnum:$number$ value:self.$name$];\n"
      "}\n");
  }


  void EnumFieldGenerator::GenerateSerializedSizeCodeHeader(io::Printer* printer) const {
  }


  void EnumFieldGenerator::GenerateSerializedSizeCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "if (self.has$capitalized_name$) {\n"
      "  size_ += computeEnumSize($number$, self.$name$);\n"
      "}\n");
  }


  void EnumFieldGenerator::GenerateDescriptionCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "if (self.has$capitalized_name$) {\n"
      "  [output appendFormat:@\"%@%@: %d\\n\", indent, @\"$name$\", self.$name$];\n"
      "}\n");
  }


  void EnumFieldGenerator::GenerateIsEqualCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "self.has$capitalized_name$ == otherMessage.has$capitalized_name$ &&\n"
      "(!self.has$capitalized_name$ || self.$name$ == otherMessage.$name$) &&");
  }


  void EnumFieldGenerator::GenerateHashCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "if (self.has$capitalized_name$) {\n"
      "  hashCode = hashCode * 31 + self.$name$;\n"
      "}\n");
  }


  string EnumFieldGenerator::GetBoxedType() const {
    return ClassName(descriptor_->enum_type());
  }


  RepeatedEnumFieldGenerator::RepeatedEnumFieldGenerator(const FieldDescriptor* descriptor)
    : descriptor_(descriptor) {
      SetEnumVariables(descriptor, &variables_);
  }


  RepeatedEnumFieldGenerator::~RepeatedEnumFieldGenerator() {
  }


  void RepeatedEnumFieldGenerator::GenerateHasFieldHeader(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GenerateFieldHeader(io::Printer* printer) const {
    printer->Print(variables_, "PBAppendableArray * $list_name$;\n");
    if (descriptor_->options().packed()) {
      printer->Print(variables_,
        "int32_t $name$MemoizedSerializedSize;\n");
    }
  }


  void RepeatedEnumFieldGenerator::GenerateHasPropertyHeader(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GeneratePropertyHeader(io::Printer* printer) const {
		//check if object array vs primitive array
		if(isObjectArray(descriptor_)){
			printer->Print(variables_, "@property (nonatomic, readonly, nullable) NSArray * $name$;\n");
		}else{
			printer->Print(variables_, "@property (nonatomic, readonly, nullable) PBArray * $name$;\n");
		}
  }

  void RepeatedEnumFieldGenerator::GenerateExtensionSource(io::Printer* printer) const {
		//check if object array vs primitive array
		if(isObjectArray(descriptor_)){
			printer->Print(variables_,
		      "@property (nonatomic, readwrite) NSMutableArray * $list_name$;\n");
		}else{
			printer->Print(variables_,
		      "@property (nonatomic, readwrite) PBAppendableArray * $list_name$;\n");
		}
  }

  void RepeatedEnumFieldGenerator::GenerateSynthesizeSource(io::Printer* printer) const {
  }

  void RepeatedEnumFieldGenerator::GenerateInitializationSource(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GenerateMembersHeader(io::Printer* printer) const {
    printer->Print(variables_,
      "- ($type$)$name$AtIndex:(NSUInteger)index;\n");
  }

  void RepeatedEnumFieldGenerator::GenerateBuilderMembersHeader(io::Printer* printer) const {
    printer->Print(variables_,
      "- ($classname$_Builder *)add$capitalized_name$:($type$)value;\n"
      "- ($classname$_Builder *)set$capitalized_name$Array:(NSArray *)array;\n");
  }

  void RepeatedEnumFieldGenerator::GenerateBuilderGetterHeader(io::Printer* printer) const {
    printer->Print(variables_,
      "- (PBAppendableArray*)$name$;\n");
  }

  void RepeatedEnumFieldGenerator::GenerateBuilderClearHeader(io::Printer* printer) const {
    printer->Print(variables_,"- (PBAppendableArray*)clear$capitalized_name$;\n");
  }

  void RepeatedEnumFieldGenerator::GenerateBuilderGetterSource(io::Printer* printer) const {
    printer->Print(variables_,
          "- (PBAppendableArray *)$name$ {\n"
          "  return builder_result.$list_name$;\n"
          "}\n");
  }

  void RepeatedEnumFieldGenerator::GenerateMergingCodeHeader(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GenerateBuildingCodeHeader(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GenerateParsingCodeHeader(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GenerateSerializationCodeHeader(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GenerateSerializedSizeCodeHeader(io::Printer* printer) const {
  }


  void RepeatedEnumFieldGenerator::GenerateMembersSource(io::Printer* printer) const {
    printer->Print(variables_,
      "- (PBArray *)$name$ {\n"
      "  return self.$list_name$;\n"
      "}\n"
      "- ($type$)$name$AtIndex:(NSUInteger)index {\n"
      "  return ($type$)[self.$list_name$ int32AtIndex:index];\n"
      "}\n");
  }

  void RepeatedEnumFieldGenerator::GenerateBuilderMembersSource(io::Printer* printer) const {
    printer->Print(variables_,
      "- ($classname$_Builder *)add$capitalized_name$:($type$)value {\n"
      "  if (builder_result.$list_name$ == nil) {\n"
      "    builder_result.$list_name$ = [PBAppendableArray arrayWithValueType:PBArrayValueTypeInt32];\n"
      "  }\n"
      "  [builder_result.$list_name$ addInt32:value];\n"
      "  return self;\n"
      "}\n"
      "- ($classname$_Builder *)set$capitalized_name$Array:(NSArray *)array {\n"
      "  builder_result.$list_name$ = [PBAppendableArray arrayWithArray:array valueType:PBArrayValueTypeInt32];\n"
      "  return self;\n"
      "}\n");
  }

  void RepeatedEnumFieldGenerator::GenerateBuilderClearSource(io::Printer* printer) const {
    printer->Print(variables_,
          "- ($classname$_Builder *)clear$capitalized_name$ {\n"
          "  builder_result.$list_name$ = nil;\n"
          "  return self;\n"
          "}\n");
  }

  void RepeatedEnumFieldGenerator::GenerateMergingCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "if (other.$list_name$.count > 0) {\n"
      "  builder_result.$list_name$ = [other.$list_name$ copy];\n"
      "}\n");
  }

  void RepeatedEnumFieldGenerator::GenerateBuildingCodeSource(io::Printer* printer) const {
  }

  void RepeatedEnumFieldGenerator::GenerateParsingCodeSource(io::Printer* printer) const {
    // If packed, set up the while loop
    if (descriptor_->options().packed()) {
      printer->Print(variables_,
        "int32_t length = [input readRawVarint32];\n"
        "int32_t oldLimit = [input pushLimit:length];\n"
        "while (input.bytesUntilLimit > 0) {\n");
      printer->Indent();
    }

    printer->Print(variables_,
      "$type$ value = ($type$)[input readEnum];\n"
      "if ($type$IsValidValue(value)) {\n"
      "  [self add$capitalized_name$:value];\n"
      "} else {\n"
      "  [unknownFields mergeVarintField:$number$ value:value];\n"
      "}\n");

    if (descriptor_->options().packed()) {
      printer->Outdent();
      printer->Print(variables_,
        "}\n"
        "[input popLimit:oldLimit];\n");
    }
  }

  void RepeatedEnumFieldGenerator::GenerateSerializationCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
        "const NSUInteger $list_name$Count = self.$list_name$.count;\n"
        "const $type$ *$list_name$Values = (const $type$ *)self.$list_name$.data;\n");

    if (descriptor_->options().packed()) {
      printer->Print(variables_,
        "if (self.$list_name$.count > 0) {\n"
        "  [output writeRawVarint32:$tag$];\n"
        "  [output writeRawVarint32:$name$MemoizedSerializedSize];\n"
        "}\n"
        "for (NSUInteger i = 0; i < $list_name$Count; ++i) {\n"
        "  [output writeEnumNoTag:$list_name$Values[i]];\n"
        "}\n");
    } else {
      printer->Print(variables_,
        "for (NSUInteger i = 0; i < $list_name$Count; ++i) {\n"
        "  [output writeEnum:$number$ value:$list_name$Values[i]];\n"
        "}\n");
    }
  }


  void RepeatedEnumFieldGenerator::GenerateSerializedSizeCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "{\n"
      "  int32_t dataSize = 0;\n"
      "  const NSUInteger count = self.$list_name$.count;\n");
    printer->Indent();

    printer->Print(variables_,
      "const $type$ *values = (const $type$ *)self.$list_name$.data;\n"
      "for (NSUInteger i = 0; i < count; ++i) {\n"
      "  dataSize += computeEnumSizeNoTag(values[i]);\n"
      "}\n");

    printer->Print(
      "size_ += dataSize;\n");

    if (descriptor_->options().packed()) {
      printer->Print(variables_,
        "if (count > 0) {\n"
        "  size_ += $tag_size$;\n"
        "  size_ += computeRawVarint32Size(dataSize);\n"
        "}\n");
    } else {
      printer->Print(variables_,
        "size_ += $tag_size$ * count;\n");
    }

    if (descriptor_->options().packed()) {
      printer->Print(variables_,
        "$name$MemoizedSerializedSize = dataSize;\n");
    }

    printer->Outdent();
    printer->Print("}\n");
  }


  void RepeatedEnumFieldGenerator::GenerateDescriptionCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "const NSUInteger $list_name$Count = self.$list_name$.count;\n"
      "if ($list_name$Count > 0) {\n"
      "  const $type$ *$list_name$Values = (const $type$ *)self.$list_name$.data;\n");
    printer->Indent();
    printer->Print(variables_,
      "for (NSUInteger i = 0; i < $list_name$Count; ++i) {\n"
      "  [output appendFormat:@\"%@%@: %d\\n\", indent, @\"$name$\", $list_name$Values[i]];\n"
      "}\n");
    printer->Outdent();
    printer->Print("}\n");
  }


  void RepeatedEnumFieldGenerator::GenerateIsEqualCodeSource(io::Printer* printer) const {
    printer->Print(variables_, "(self.$list_name$ == otherMessage.$list_name$ || [self.$list_name$ isEqualToArray:otherMessage.$list_name$]) &&");
  }


  void RepeatedEnumFieldGenerator::GenerateHashCodeSource(io::Printer* printer) const {
    printer->Print(variables_,
      "const NSUInteger $list_name$Count = self.$list_name$.count;\n"
      "if ($list_name$Count > 0) {\n"
      "  const $type$ *$list_name$Values = (const $type$ *)self.$list_name$.data;\n");
    printer->Indent();
    printer->Print(variables_,
      "for (NSUInteger i = 0; i < $list_name$Count; ++i) {\n"
      "  hashCode = hashCode * 31 + $list_name$Values[i];\n"
      "}\n");
    printer->Outdent();
    printer->Print("}\n");
  }
}  // namespace objectivec
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
