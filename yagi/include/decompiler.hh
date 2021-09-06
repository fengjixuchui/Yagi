#ifndef __YAGI_IDECOMPILE__
#define __YAGI_IDECOMPILE__

#include <string>
#include <map>
#include <optional>

namespace yagi 
{
	/*!
	 * \brief Memory location
	 */
	struct MemoryLocation
	{
		enum class MemoryLocationType
		{
			RAM,
			Register,
			Stack
		};

		MemoryLocationType type;
		uint64_t offset;
		uint32_t addrSize;
		uint64_t pc;
		uint64_t typeSize;

		MemoryLocation(MemoryLocationType type, uint64_t offset, uint32_t addrSize, uint64_t pc = 0, uint64_t tyepSz = 0)
			: type{ type }, offset{ offset }, addrSize{ addrSize }, pc { pc }, typeSize { tyepSz }
		{}


		MemoryLocation(const std::string& name, uint64_t offset, uint32_t addrSize, uint64_t pc = 0, uint64_t tyepSz = 0)
			: offset{ offset }, addrSize { addrSize }, pc{ pc }, typeSize{ tyepSz }
		{
			if (name == "register")
			{
				type = MemoryLocationType::Register;
			}
			else if (name == "stack")
			{
				type = MemoryLocationType::Stack;
			}
			else {
				type = MemoryLocationType::RAM;
			}
		}
	};

	/*!
	 * \brief	Compiler configuration definition
	 */
	struct Compiler {
		/*!
		 * \brief	Language definition 
		 */
		enum class Language {
			X86_WINDOWS,	// built using Windows
			X86_GCC,		// built using GCC
			X86,			// unknown compiler
			ARM,			// ARM	arch
			PPC,			// PowerPC
			MIPS,			// MIPS
			SPARC			// Sparc
		};

		enum class Endianess {
			BE,				// Big Endian
			LE				// Little Endian
		};

		enum class Mode {
			M32,			// 32 bits
			M64				// 64 bits
		};

		Language	language;
		Endianess	endianess;
		Mode		mode;

		/*!
		 * \brief	Constructor
		 */
		Compiler(Language language, Endianess endianess, Mode mode)
			: language {language}, endianess{endianess}, mode{mode}
		{}
	};

	/*!
	 * \brief	Decompile interface
	 */
	class Decompiler
	{
	public:

		/*!
		 * \brief	result of the decompiler 
		 */
		struct Result
		{
			/*!
			 * \brief	raw code formated with correct synthax
			 */
			std::string	cCode;

			/*!
			 * \brief	name of the function
			 */
			std::string name;

			/*!
			 * \brief	address of function
			 */
			uint64_t ea;

			/*!
			 * \brief	a mapping between token symbol and address
			 */
			std::map<std::string, MemoryLocation> symbolAddress;

			/*!
			 * \brief	ctor
			 */
			Result(std::string name, uint64_t ea, std::string cCode, std::map<std::string, MemoryLocation> symbolAddress)
				: name{ name }, ea { ea }, cCode{ cCode }, symbolAddress{ symbolAddress }
			{}
		};

		virtual ~Decompiler() = default;

		/*!
		 * \brief	decompiler function interface
		 * \param	funcAddress	address of the function to decompile
		 * \return	decompiled source code
		 */
		virtual std::optional<Result> decompile(uint64_t funcAddress) = 0;
	};
}

#endif