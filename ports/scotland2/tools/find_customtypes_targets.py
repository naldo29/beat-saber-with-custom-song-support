#!/usr/bin/env python3
"""Follow pinned bshook/custom-types source lookups in original game ELFs."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[3]/'tools'))
from audit_elf import Elf,cstr
from prepare_hooks import signed

def profile(il2cpp_path,unity_path):
    elf=Elf(str(il2cpp_path));unity=Elf(str(unity_path));symbols={}
    for table in elf.sh:
        if table[1]!=11:continue
        strings=elf.sh[table[6]][4]
        for at in range(table[4],table[4]+table[5],24):
            name,info,other,index,address,size=struct.unpack_from('<IBBHQQ',elf.data,at)
            if index and info&15==2:symbols[cstr(elf.data,strings+name)]=address
    trace=[]
    def word(image,address):
        assert any(s[1]==1 and s[2]&4 and s[3]<=address<=s[3]+s[5]-4 for s in image.sh),hex(address)
        return struct.unpack_from('<I',image.data,image._off(address))[0]
    def nth(address,n,link=False,limit=4096):
        initial=address
        for pc in range(address,address+limit,4):
            w=word(elf,pc)
            if w>>26==(0b100101 if link else 0b000101):
                n-=1
                if not n:
                    target=pc+signed(w&0x3ffffff,26)*4;word(elf,target)
                    trace.append({'start':hex(initial),'instruction':hex(pc),'target':hex(target),'kind':'BL' if link else 'B'})
                    return target
        raise ValueError('bounded branch lookup failed at '+hex(initial))
    targets={}
    targets['Class_FromIl2CppType']=nth(symbols['il2cpp_class_from_il2cpp_type'],1,limit=1024)
    image_type=nth(symbols['il2cpp_image_get_class'],1,limit=1024)
    metadata_handle=nth(image_type,1,limit=1024)
    targets['GlobalMetadata_GetTypeInfoFromHandle']=nth(metadata_handle,1,limit=1024)
    targets['GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex']=nth(targets['GlobalMetadata_GetTypeInfoFromHandle'],1,limit=1024)
    targets['Class_Init']=nth(nth(symbols['il2cpp_array_new_specific'],1),1,True)
    targets['Type_GetClassOrElementClass']=nth(symbols['il2cpp_type_get_class_or_element_class'],1)
    root=symbols.get('il2cpp_unity_liveness_calculation_from_root')
    if root and word(elf,root)>>26==0b000101:
        generic=nth(nth(nth(root,1),2,True),1,True)
        targets['LivenessState_TraverseGenericObject']=generic
        targets['LivenessState_TraverseGCDescriptor']=nth(generic,2)
        targets['LivenessState_TraverseObjectInternal']=nth(generic,3)
    pattern=bytes.fromhex('ff 43 02 d1 fe 23 00 f9 fa 67 05 a9 f8 5f 06 a9 f6 57 07 a9 f4 4f 08 a9 57 d0 3b d5 e8 16 40 f9 f6 03 01 aa')
    matches=[]
    for s in unity.sh:
        if s[1]!=1 or not s[2]&4:continue
        data=unity.data[s[4]:s[4]+s[5]];pos=data.find(pattern)
        while pos>=0:
            matches.append(s[3]+pos);pos=data.find(pattern,pos+1)
    if len(matches)!=1:raise ValueError('GetScriptingClass signature is not unique: '+str(matches))
    def rows(image,items):return [{'name':name,'vaddr':hex(va),'expect':hex(word(image,va))} for name,va in items.items()]
    return {'schema':1,'sources':['bshook 6.4.2 il2cpp-functions.cpp','custom-types 0.18.3 register.cpp'],
        'il2cpp':{'sha256':hashlib.sha256(elf.data).hexdigest(),'targets':rows(elf,targets),'trace':trace},
        'unity':{'sha256':hashlib.sha256(unity.data).hexdigest(),'pattern':pattern.hex(),'targets':rows(unity,{'GetScriptingClass':matches[0]})}}

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--il2cpp',type=Path,required=True)
    p.add_argument('--unity',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();result=profile(a.il2cpp,a.unity);a.output.write_text(json.dumps(result,indent=2)+'\n')
    for image in ['il2cpp','unity']:
        for row in result[image]['targets']:print(image,row)
