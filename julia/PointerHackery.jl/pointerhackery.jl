using StaticArrays

include("offsetof.jl")

memcpy(dst, src, size) = ccall(:memcpy, Ptr{Void}, (Ptr{Void}, Ptr{Void}, UInt64), dst, src, size)
free(address) = ccall((:my_free, appdll), Void, (Ptr{Void}, ), address)
realloc(oldmemory, newsize) = ccall(:realloc, Ptr{Void}, (Ptr{Void}, UInt64), oldmemory, newsize)

# UnsafePtr :> Ptr
# nfields(foo6)
# GetStructType(foo6, :next)
# Core.fieldtype(foo6, :next)


# warning: I dont recall anymore why ive choosen one based indexing for Craft pointerhackery... but IMO it makes no sense anymore, so instead of i-1 I do i-0... fix if I need it again

function Base.getindex(ptr::Ptr{T}, index::Int) where {T}
  address = UInt(ptr)
  if address == 0
    throw("Base.getindex(Ptr::{$T}) would dereference a NULL pointer")
  end
  offset = sizeof(T) * (index-0) # convert 0 based to 1 based indexing
  # UInt(100) + Int(-20) is fuckin inexact error... so lets use Int, which sux for pointer values
  fieldptr = Ptr{T}(Int(address) + offset)
  #print("Index=$index address=$address ptr=$ptr fieldptr=$fieldptr\n")
  #return 123
  #return fieldptr
  return unsafe_load(fieldptr)
end

function Base.getindex(ptr::Ptr{SVector{N, T}}, index::Int) where {N, T}
  address = UInt(ptr)
  if address == 0
    throw("Base.getindex(Ptr::{$T}) would dereference a NULL pointer")
  end
  if ! (1 <= index <= N)
    throw("out of bounds, need 1 <= index=$index <= N=$N\n")
  end
  offset = sizeof(T) * (index-0) # convert 0 based to 1 based indexing
  # UInt(100) + Int(-20) is fuckin inexact error... so lets use Int, which sux for pointer values
  fieldptr = Ptr{T}(Int(address) + offset)
  #log("Index=$index address=$address ptr=$ptr fieldptr=$fieldptr\n")
  #return 123
  #return fieldptr
  #return unsafe_load(fieldptr)


	#log("FUCK YOU $fieldtype $iss\n")
	# give address to "types" and load values from float, int etc.
	if T <: SVector || supertype(T) == Any
		return fieldptr
	else
		return unsafe_load(fieldptr)
	end
end

function Base.getindex(ptr::Ptr{T}, s::Symbol) where {T}
  address = UInt(ptr)
  if address == 0
    throw("Base.getindex(Ptr::{$T}) would dereference a NULL pointer")
  end
  offset = offsetof(T, s)
  fieldtype = Core.fieldtype(T, s)
  fieldptr = Ptr{fieldtype}(address + offset)
  #log("Symbol $s $ptrtype address=$address offset=$offset fieldtype=$fieldtype ptr=$ptr fieldptr=$fieldptr\n")
  #return 123
  #
	#if isbits(fieldtype)

	#iss  = fieldtype <: SVector
	#log("FUCK YOU $fieldtype $iss\n")
	if fieldtype <: SVector
		return fieldptr
	end
	if supertype(fieldtype) != Any
		return unsafe_load(fieldptr)
	else
		return fieldptr # only return pointers, otherwise the giant static arrays are loaded, even tho i just wanna peak in them
	end
end

function Base.setindex!(ptr::Ptr{T}, value, s::Symbol) where {T}
#	log("fuck off\n")
  address = UInt(ptr)
  if address == 0
    throw("Base.setindex!(Ptr) would write to a NULL pointer")
  end
  offset = offsetof(T, s)
  fieldtype = Core.fieldtype(T, s)
  fieldptr = Ptr{fieldtype}(address + offset)
  #log("Symbol $s $ptrtype address=$address offset=$offset fieldtype=$fieldtype ptr=$ptr fieldptr=$fieldptr\n")
  unsafe_store!(fieldptr, value)
  return value
end

function Base.setindex!(ptr::Ptr{SVector{N, T}}, value, index) where {N, T}
	#log("fuck you $T\n")
	if ptr == 0
		throw("Base.setindex!(Ptr) would write to a NULL pointer")
	end
	offset = sizeof(T) * (index-0) # compensate for 1 based indexing
	finaladdress = Ptr{T}( ptr + offset )
	#log("$ptr $value $index $offset $finaladdress\n")

	# todo: handle SVector in SVector?
	unsafe_store!(finaladdress, value)
	#return value
	#return Float32(122)
end


function Base.setindex!(ptr::Ptr{T}, newvalue, atindex) where {T}
	offset = sizeof(T) * (atindex-0) # compensate for 1 based indexing
	fieldptr = Ptr{T}(ptr + offset)
	unsafe_store!(fieldptr, newvalue)
end

function topointer(object)
	ptr = pointer_from_objref(object)
	return Ptr{typeof(object)}(ptr)
end

# turn array object into svector pointer
function topointer(array::Array{Float32, 1})
	return Ptr{SVector{length(array), Float32}}( pointer(array) )
end

# I guess this is wrong... one time it worked, and other not, gotta still test
function Base.pointer(vector::SVector{N, T}) where {N, T}
	return Ptr{ SVector{N, T} }( pointer_from_objref(vector) )
end

if false
	f = Float32(123)
	ptr = topointer(f)
	ptr[0] += 233
end

if false
  craft_struct() = ccall(("craft_struct" , "C:\\deepdreamers\\julia\\opsys.dll"), Int64, ())
  immutable foo6
    a::Int32
    b::Int32
    next::Ptr{foo6}
  end
  meh = Ptr{foo6}(craft_struct())
  C_NULL[:asd] # throws error, since it would dereference the pointer
  meh[:next][:next][:next][:next]
  meh[:a] = 222222
  #[:next][:next][:next] 
end

function malloc(the_type, count=1)
  address = ccall(:malloc, (Int64), (Int64, ), sizeof(the_type) * count)
  return Ptr{the_type}(address)
  #log("typesize " * string(sizeof(the_type)) * "\n")
end

# void * memset ( void * ptr, int value, size_t num );
function memset(ptr, value, num)
  ccall(:memset, Int64, (Int64, Int32, Int64), ptr, value, num)
end

function memset(object, value)
  memset(object, value, sizeof(eltype(object)))
end

function addressof(object)
  return Ptr{ typeof(object) }( pointer_from_objref(object) )
end

if false
  #test = malloc(foo5)
  #unsafe_load(test).a = Int32(123)
  #memset(test, 0, sizeof(foo5))
  #unsafe_load(test)
  #sizeof(test)
  sizeof(ptrtype(addressof(b)))
  addr = Int64(addressof(b))
  memset(addr, 123, 1)
  somechar = UInt8(123)
  someptr = Ptr{somechar}
  sizeof(a)
  sizeof(somechar)
  eltype(a)
  typeof(a)
  a = UInt8(123)
  b = UInt8(222)
  hex(Int64(addressof(b)))
  memset(addressof(b), 255)
  methods(pointer_from_objref)
end

function topointer(this::String)
	log("meh\n")
	ptr = pointer(this)
	len = length(this)
	return Ptr{SVector{len,UInt8}}(ptr)
end

if false
	str = "test"
	ptr = topointer(str)
	ptr[1] = 65
	str # Aest
end


