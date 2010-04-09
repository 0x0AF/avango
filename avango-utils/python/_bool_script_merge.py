import avango
import avango.script

import _bool_scripts

def merge_and_connect_bool_scripts(script1, script2):
    '''
    Merges the SFBool nodes a two avango.FieldContainers with an OR relation
    '''
    merged_script = avango.script.Script()
    
    for i in xrange(script1._get_num_fields()):
        #get field of script1
        script1_field = script1._get_field(i)
        field1_name = script1_field._get_name()
        field1_type = script1_field._get_type()
        
        #get corresponding field of script2
        script2_field = script2._get_field(field1_name)
        field2_type = script1_field._get_type()
        
        #check if script2 also has a filed with this name and of the same type
        if script2_field == None and field1_type == field2_type:
            continue
        
        #check if the type of the field is not SFBool and 
        #the merged script already does not already contain a field with the given name
        if field1_type != "SFBool" and merged_script._get_field(field1_name):
            continue 
        
        new_field = avango._avango._make_field_by_name(script1_field._get_type())
        merged_script.add_field(new_field, field1_name)

        # connect new field from node's field
        # use an OR relationship
        new_field = merged_script._get_field(field1_name)    
        new_field.connect_from(_bool_scripts.make_bool2_or(script1_field,script2_field))
        
    return merged_script