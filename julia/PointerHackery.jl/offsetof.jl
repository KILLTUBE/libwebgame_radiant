
# as optimized as it probably can be: https://stackoverflow.com/questions/44668696/optimizing-offsetof-down-to-a-constant-value/44680092
Base.@pure offsetof(type_, member::Symbol) = fieldoffset(type_, Base.fieldindex(type_, member))

#function offsetof(type_, member::Symbol)
#  for (i, item) in enumerate(fieldnames(type_))
#    if item == member
#      return fieldoffset(type_, i)
#    end
#    #print(typeof(i))
#  end
#  # what to do when symbol not in type_?
#  throw("$type_ has no member named $member")
#end

# use Core.fieldtype(ABC, :b) instead
#function GetStructType(type_, member::Symbol)
#  for (i, item) in enumerate(fieldnames(type_))
#    if item == member
#      return fieldtype(type_, i)
#    end
#    #print(typeof(i))
#  end
#  # what to do when symbol not in type_?
#  throw("$type_ has no member named $member")
#end

if false
  #@code_native offsetof(ABC, :c)
    type ABC a::Int64; b::Int64; c::Int64 end
  offsetof(ABC, :a) # 0
  offsetof(ABC, :b) # 8
  offsetof(ABC, :c) # 16
  @code_llvm GetStructType(ABC, :c)
  GetStructType(ABC, :c) # 
end
