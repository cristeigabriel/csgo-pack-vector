#pragma once

#include <cstdint>
#include <cmath>
#include <type_traits>

namespace detail
{
	template <typename T>
	concept args_t = std::is_integral<T>::value || std::is_floating_point<T>::value;
}

template <detail::args_t T>
struct vector_t
{
	template <detail::args_t... Args>
	struct pack
	{
		public:
		constexpr static size_t Len = sizeof...( Args );
		using Arr                   = std::array<T, Len>;

		//	============================================================================================

		explicit pack( ) = default;

		auto initialize( const Args& ...args )->void
		{
			m_contents = Arr{ {args... } };
		}

		explicit pack( const Args& ...args )
		{
			initialize( args... );
		}

		//	============================================================================================

		//	============================================================================================
		//	Operational methods
		//	============================================================================================

		/**
		 * Len grabber
		 * 
		 * \return 
		 */
		constexpr auto get_size( )const->size_t
		{
			return Len;
		}

		/**
		 * Grab Nth member of contents
		 *
		 * \param i
		 * \return
		 */
		auto operator[ ]( const size_t i )const->T
		{
			return m_contents[ i ];
		}

		/**
		 * Actually access Nth of contents
		 *
		 * \param i
		 * \return
		 */
		auto operator[ ]( const size_t i )->T &
		{
			return m_contents[ i ];
		}

		/**
		 * Error handled operator[]
		 * Grab Nth member of contents
		 *
		 * \param i
		 * \return
		 */
		auto at( const size_t i )const->T
		{
			assert( i < Len );
			return operator[]( i );
		}

		auto at( const size_t i )->T &
		{
			assert( i < Len );
			return operator[]( i );
		}

		/**
		 * Grab const instance of
		 * contents in C++ array style (fixed size)
		 *
		 * \return
		 */
		auto get_contents( )const->const Arr
		{
			return m_contents;
		}

		/**
		 * Copy of self grabber
		 *
		 * \return
		 */
		auto get_copy( )const->pack<Args...>
		{
			auto _this = *this;
			return _this;
		}

		//	============================================================================================

		//	============================================================================================
		//	Mathematical methods
		//	============================================================================================

		template <size_t N = Len>
		auto get_dot( )const->T
		{
			static_assert( N <= Len );

			const auto &contents = get_contents( );
			auto result          = T{};

			for( auto i = size_t{ 0 }; i < N; ++i )
			{
				result += contents[ i ] * contents[ i ];
			}

			return result;
		}

		template <size_t N = Len>
		auto get_length( )const
		{
			return sqrt( get_dot<N>( ) );
		}

		template <size_t N = Len>
		auto dot( T arg )const->T
		{
			static_assert( N <= Len );

			const auto &contents = get_contents( );
			auto result          = T{};

			for( auto i = size_t{ 0 }; i < N; ++i )
			{
				result += contents[ i ] * arg;
			}

			return result;
		}

		template <size_t N = Len>
		auto dot( const pack &arg )const->T
		{
			static_assert( N <= Len );
			assert( arg.get_size( ) <= N );

			const auto &arg_contents = get_contents( );
			const auto &contents     = get_contents( );
			auto result              = T{};

			for( auto i = size_t{ 0 }; i < N; ++i )
			{
				result += contents[ i ] * arg_contents[ i ];
			}

			return result;
		}

		template <size_t N = Len>
		auto length( T arg )const
		{
			return sqrt( dot<N>( arg ) );
		}

		template <size_t N = Len>
		auto length( const pack &arg )const
		{
			return sqrt( dot<N>( arg ) );
		}

		//	============================================================================================

		//	============================================================================================
		//	CS:GO methods
		//	============================================================================================

		auto normalize( )->void
		{
			static_assert( std::is_same_v<T, float> );
			static_assert( Len == 3U );

			const auto &length = get_length( );

			//	[0] = Pitch
			//	[1] = Yaw
			//	[2] = Roll

			if( length != 0.F )
			{
				operator[]( 0u ) /= length;
				operator[]( 1u ) /= length;
				operator[]( 2u ) /= length;
			}
			else
			{
				operator[]( 0u ) = operator[]( 1u ) = 0.F;
				operator[]( 2u )  = 1.F;
			}
		}

		auto normalized( )const->pack<Args...>
		{
			static_assert( std::is_same_v<T, float> );
			static_assert( Len == 3U );

			const auto &copy   = get_copy( );
			const auto &length = copy.get_length( );

			//	[0] = Pitch
			//	[1] = Yaw
			//	[2] = Roll

			if( length != 0.F )
			{
				copy[ 0 ] /= length;
				copy[ 1 ] /= length;
				copy[ 2 ] /= length;
			}
			else
			{
				copy[ 0 ] = copy[ 1 ] = 0.F;
				copy[ 2 ] = 1.F;
			}

			return copy;
		}

		//	============================================================================================

		private:
		Arr m_contents = Arr{};
	};

	using v2 = pack<T, T>;
	using v3 = pack<T, T, T>;
	using v4 = pack<T, T, T, T>;
};
