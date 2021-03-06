/*
   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _VECTORMATH_QUAT_AOS_CPP_H
#define _VECTORMATH_QUAT_AOS_CPP_H


//-----------------------------------------------------------------------------
// Definitions

#ifndef _VECTORMATH_INTERNAL_FUNCTIONS
#define _VECTORMATH_INTERNAL_FUNCTIONS

#endif

namespace VML {

#ifdef _DEBUG  // Debug versions check alignment before initialization

inline Quat::Quat( float _x, float _y, float _z, float _w )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = _mm_setr_ps(_x, _y, _z, _w);
}

inline Quat::Quat( const floatInVec &_x, const floatInVec &_y, const floatInVec &_z, const floatInVec &_w )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = _mm_unpacklo_ps(
		_mm_unpacklo_ps( _x.get128(), _z.get128() ),
		_mm_unpacklo_ps( _y.get128(), _w.get128() ) );
}

inline Quat::Quat( const Vector3 &xyz, float _w )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = xyz.get128();
    _vmathVfSetElement(mVec128, _w, 3);
}

inline Quat::Quat( const Vector3 &xyz, const floatInVec &_w )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = xyz.get128();
    mVec128 = _vmathVfInsert(mVec128, _w.get128(), 3);
}

inline Quat::Quat( const Vector4 &vec )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = vec.get128();
}

inline Quat::Quat( float scalar )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = _mm_set1_ps(scalar);
}

inline Quat::Quat( const floatInVec &scalar )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = scalar.get128();
}

inline Quat::Quat( const __m128& vf4 )
{
	_CHECK_ALIGN(mVec128, 16);
	mVec128 = vf4;
}

#else

inline Quat::Quat( float _x, float _y, float _z, float _w ) : mVec128( _mm_setr_ps(_x, _y, _z, _w) ) { }

inline Quat::Quat( const floatInVec &_x, const floatInVec &_y, const floatInVec &_z, const floatInVec &_w )
{
	mVec128 = _mm_unpacklo_ps(
		_mm_unpacklo_ps( _x.get128(), _z.get128() ),
		_mm_unpacklo_ps( _y.get128(), _w.get128() ) );
}

inline Quat::Quat( const Vector3 &xyz, float _w ) : mVec128( xyz.get128() )
{ _vmathVfSetElement(mVec128, _w, 3); }

inline Quat::Quat( const Vector3 &xyz, const floatInVec &_w ) : mVec128( xyz.get128() )
{ mVec128 = _vmathVfInsert(mVec128, _w.get128(), 3); }

inline Quat::Quat( const Vector4 &vec ) : mVec128( vec.get128() ) { }

inline Quat::Quat( float scalar ) : mVec128( _mm_set1_ps(scalar) ) { }

inline Quat::Quat( const floatInVec &scalar ) : mVec128( scalar.get128() ) { }

inline Quat::Quat( const __m128& vf4 ) : mVec128( vf4 ) { }

#endif

inline const Quat Quat::identity( )
{
    return Quat( _VECTORMATH_UNIT_0001 );
}

inline const Quat lerp( float t, const Quat &quat0, const Quat &quat1 )
{
    return lerp( floatInVec(t), quat0, quat1 );
}

inline const Quat lerp( const floatInVec &t, const Quat &quat0, const Quat &quat1 )
{
    return ( quat0 + ( ( quat1 - quat0 ) * t ) );
}

inline const Quat slerp( float t, const Quat &unitQuat0, const Quat &unitQuat1 )
{
    return slerp( floatInVec(t), unitQuat0, unitQuat1 );
}

inline const Quat slerp( const floatInVec &t, const Quat &unitQuat0, const Quat &unitQuat1 )
{
    Quat start;
    vec_float4 scales, scale0, scale1, cosAngle, angle, tttt, oneMinusT, angles, sines;
    vec_uint4 selectMask;
    cosAngle = _vmathVfDot4( unitQuat0.get128(), unitQuat1.get128() );
    selectMask = (vec_uint4)vec_cmpgt( _mm_setzero_ps(), cosAngle );
    cosAngle = vec_sel( cosAngle, negatef4( cosAngle ), selectMask );
    start = Quat( vec_sel( unitQuat0.get128(), negatef4( unitQuat0.get128() ), selectMask ) );
    selectMask = (vec_uint4)vec_cmpgt( _mm_set1_ps(_VECTORMATH_SLERP_TOL), cosAngle );
    angle = acosf4( cosAngle );
    tttt = t.get128();
    oneMinusT = vec_sub( _mm_set1_ps(1.0f), tttt );
    angles = vec_mergeh( _mm_set1_ps(1.0f), tttt );
    angles = vec_mergeh( angles, oneMinusT );
    angles = vec_madd( angles, angle, _mm_setzero_ps() );
    sines = sinf4( angles );
    scales = _mm_div_ps( sines, vec_splat( sines, 0 ) );
    scale0 = vec_sel( oneMinusT, vec_splat( scales, 1 ), selectMask );
    scale1 = vec_sel( tttt, vec_splat( scales, 2 ), selectMask );
    return Quat( vec_madd( start.get128(), scale0, vec_mul( unitQuat1.get128(), scale1 ) ) );
}

inline const Quat squad( float t, const Quat &unitQuat0, const Quat &unitQuat1, const Quat &unitQuat2, const Quat &unitQuat3 )
{
    return squad( floatInVec(t), unitQuat0, unitQuat1, unitQuat2, unitQuat3 );
}

inline const Quat squad( const floatInVec &t, const Quat &unitQuat0, const Quat &unitQuat1, const Quat &unitQuat2, const Quat &unitQuat3 )
{
    return slerp( ( ( floatInVec(2.0f) * t ) * ( floatInVec(1.0f) - t ) ), slerp( t, unitQuat0, unitQuat3 ), slerp( t, unitQuat1, unitQuat2 ) );
}

inline const __m128& Quat::get128( ) const
{
    return mVec128;
}

inline Quat & Quat::operator =( const Quat &quat )
{
    mVec128 = quat.mVec128;
    return *this;
}

inline Quat & Quat::setXYZ( const Vector3 &vec )
{
	ALIGN_EPILOG(16) unsigned int sw[4] ALIGN_EPILOG(16) = {0, 0, 0, 0xffffffff};
	mVec128 = vec_sel( vec.get128(), mVec128, sw );
    return *this;
}

inline const Vector3 Quat::getXYZ( ) const
{
    return Vector3( mVec128 );
}

inline void Quat::setX( float _x ) { _vmathVfSetElement(mVec128, _x, 0); }

inline void Quat::setX( const floatInVec &_x ) { mVec128 = _vmathVfInsert(mVec128, _x.get128(), 0); }

inline const floatInVec Quat::getX( ) const { return floatInVec( mVec128, 0 ); }

inline float Quat::getXf( ) const { return getX(); }

inline void Quat::setY( float _y ) { _vmathVfSetElement(mVec128, _y, 1); }

inline void Quat::setY( const floatInVec &_y ) { mVec128 = _vmathVfInsert(mVec128, _y.get128(), 1); }

inline const floatInVec Quat::getY( ) const { return floatInVec( mVec128, 1 ); }

inline float Quat::getYf( ) const { return getY(); }

inline void Quat::setZ( float _z ) { _vmathVfSetElement(mVec128, _z, 2); }

inline void Quat::setZ( const floatInVec &_z ) { mVec128 = _vmathVfInsert(mVec128, _z.get128(), 2); }

inline const floatInVec Quat::getZ( ) const { return floatInVec( mVec128, 2 ); }

inline float Quat::getZf( ) const { return getZ(); }

inline void Quat::setW( float _w ) { _vmathVfSetElement(mVec128, _w, 3); }

inline void Quat::setW( const floatInVec &_w ) { mVec128 = _vmathVfInsert(mVec128, _w.get128(), 3); }

inline const floatInVec Quat::getW( ) const { return floatInVec( mVec128, 3 ); }

inline float Quat::getWf( ) const { return getW(); }

inline void Quat::setElem( int idx, float value ) { _vmathVfSetElement(mVec128, value, idx); }

inline void Quat::setElem( int idx, const floatInVec &value ) { mVec128 = _vmathVfInsert(mVec128, value.get128(), idx); }

inline const floatInVec Quat::getElem( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline VecIdx Quat::operator []( int idx )
{
    return VecIdx( mVec128, idx );
}

inline const floatInVec Quat::operator []( int idx ) const
{
    return floatInVec( mVec128, idx );
}

inline const Quat Quat::operator +( const Quat &quat ) const
{
    return Quat( _mm_add_ps( mVec128, quat.mVec128 ) );
}

inline const Quat Quat::operator -( const Quat &quat ) const
{
    return Quat( _mm_sub_ps( mVec128, quat.mVec128 ) );
}

inline const Quat Quat::operator *( float scalar ) const
{
    return *this * floatInVec(scalar);
}

inline const Quat Quat::operator *( const floatInVec &scalar ) const
{
    return Quat( _mm_mul_ps( mVec128, scalar.get128() ) );
}

inline Quat & Quat::operator +=( const Quat &quat )
{
    *this = *this + quat;
    return *this;
}

inline Quat & Quat::operator -=( const Quat &quat )
{
    *this = *this - quat;
    return *this;
}

inline Quat & Quat::operator *=( float scalar )
{
    *this = *this * scalar;
    return *this;
}

inline Quat & Quat::operator *=( const floatInVec &scalar )
{
    *this = *this * scalar;
    return *this;
}

inline const Quat Quat::operator /( float scalar ) const
{
    return *this / floatInVec(scalar);
}

inline const Quat Quat::operator /( const floatInVec &scalar ) const
{
    return Quat( _mm_div_ps( mVec128, scalar.get128() ) );
}

inline Quat & Quat::operator /=( float scalar )
{
    *this = *this / scalar;
    return *this;
}

inline Quat & Quat::operator /=( const floatInVec &scalar )
{
    *this = *this / scalar;
    return *this;
}

inline const Quat Quat::operator -( ) const
{
	return Quat(_mm_sub_ps( _mm_setzero_ps(), mVec128 ) );
}

inline const floatInVec Quat::norm( ) const
{
	return floatInVec(  _vmathVfDot4( mVec128, mVec128 ), 0 );
}

inline const floatInVec Quat::length( ) const
{
	return floatInVec(  _mm_sqrt_ps(_vmathVfDot4( mVec128, mVec128 )), 0 );
}

inline void Quat::normalize( )
{
	mVec128 = _mm_mul_ps( mVec128, _mm_rsqrt_ps( _vmathVfDot4( mVec128, mVec128 ) ) );
}

inline const Quat operator *( float scalar, const Quat &quat )
{
    return floatInVec(scalar) * quat;
}

inline const Quat operator *( const floatInVec &scalar, const Quat &quat )
{
    return quat * scalar;
}

inline const floatInVec dot( const Quat &quat0, const Quat &quat1 )
{
    return floatInVec( _vmathVfDot4( quat0.get128(), quat1.get128() ), 0 );
}

inline const Quat normalize( const Quat &quat )
{
    return Quat( _mm_mul_ps( quat.get128(), _mm_rsqrt_ps( _vmathVfDot4( quat.get128(), quat.get128() ) ) ) );
}

inline const Quat Quat::rotation( const Vector3 &unitVec0, const Vector3 &unitVec1 )
{
    Vector3 crossVec;
    __m128 cosAngle, cosAngleX2Plus2, recipCosHalfAngleX2, cosHalfAngleX2, res;
    cosAngle = _vmathVfDot3( unitVec0.get128(), unitVec1.get128() );
    cosAngleX2Plus2 = vec_madd( cosAngle, _mm_set1_ps(2.0f), _mm_set1_ps(2.0f) );
    recipCosHalfAngleX2 = _mm_rsqrt_ps( cosAngleX2Plus2 );
    cosHalfAngleX2 = vec_mul( recipCosHalfAngleX2, cosAngleX2Plus2 );
    crossVec = cross( unitVec0, unitVec1 );
    res = vec_mul( crossVec.get128(), recipCosHalfAngleX2 );
	ALIGN_PROLOG(16) unsigned int sw[4] ALIGN_EPILOG(16) = {0, 0, 0, 0xffffffff};
    res = vec_sel( res, vec_mul( cosHalfAngleX2, _mm_set1_ps(0.5f) ), sw );
    return Quat( res );
}

inline const Quat Quat::rotation( float radians, const Vector3 &unitVec )
{
    return rotation( floatInVec(radians), unitVec );
}

inline const Quat Quat::rotation( const floatInVec &radians, const Vector3 &unitVec )
{
    __m128 s, c, angle, res;
    angle = vec_mul( radians.get128(), _mm_set1_ps(0.5f) );
    sincosf4( angle, &s, &c );
	ALIGN_PROLOG(16) unsigned int sw[4] ALIGN_EPILOG(16) = {0, 0, 0, 0xffffffff};
    res = vec_sel( vec_mul( unitVec.get128(), s ), c, sw );
    return Quat( res );
}

inline const Quat Quat::rotationX( float radians )
{
    return rotationX( floatInVec(radians) );
}

inline const Quat Quat::rotationX( const floatInVec &radians )
{
    __m128 s, c, angle, res;
    angle = vec_mul( radians.get128(), _mm_set1_ps(0.5f) );
    sincosf4( angle, &s, &c );
	ALIGN_PROLOG(16) unsigned int xsw[4] ALIGN_EPILOG(16) = {0xffffffff, 0, 0, 0};
	ALIGN_PROLOG(16) unsigned int wsw[4] ALIGN_EPILOG(16) = {0, 0, 0, 0xffffffff};
    res = vec_sel( _mm_setzero_ps(), s, xsw );
    res = vec_sel( res, c, wsw );
    return Quat( res );
}

inline const Quat Quat::rotationY( float radians )
{
    return rotationY( floatInVec(radians) );
}

inline const Quat Quat::rotationY( const floatInVec &radians )
{
    __m128 s, c, angle, res;
    angle = vec_mul( radians.get128(), _mm_set1_ps(0.5f) );
    sincosf4( angle, &s, &c );
	ALIGN_PROLOG(16) unsigned int ysw[4] ALIGN_EPILOG(16) = {0, 0xffffffff, 0, 0};
	ALIGN_PROLOG(16) unsigned int wsw[4] ALIGN_EPILOG(16) = {0, 0, 0, 0xffffffff};
    res = vec_sel( _mm_setzero_ps(), s, ysw );
    res = vec_sel( res, c, wsw );
    return Quat( res );
}

inline const Quat Quat::rotationZ( float radians )
{
    return rotationZ( floatInVec(radians) );
}

inline const Quat Quat::rotationZ( const floatInVec &radians )
{
    __m128 s, c, angle, res;
    angle = vec_mul( radians.get128(), _mm_set1_ps(0.5f) );
    sincosf4( angle, &s, &c );
	ALIGN_PROLOG(16) unsigned int zsw[4] ALIGN_EPILOG(16) = {0, 0, 0xffffffff, 0};
	ALIGN_PROLOG(16) unsigned int wsw[4] ALIGN_EPILOG(16) = {0, 0, 0, 0xffffffff};
    res = vec_sel( _mm_setzero_ps(), s, zsw );
    res = vec_sel( res, c, wsw );
    return Quat( res );
}

inline const Quat Quat::lookAt( const VML::Vector3 &dir, const VML::Vector3 &up )
{
	/// \todo optimize
	return Quat(Matrix4::lookAtLH(Point3(0), Point3(dir), up).getUpper3x3());
}

inline const Quat Quat::operator *( const Quat &quat ) const
{
    __m128 ldata, rdata, qv, tmp0, tmp1, tmp2, tmp3;
    __m128 product, l_wxyz, r_wxyz, xy, qw;
    ldata = mVec128;
    rdata = quat.mVec128;
    tmp0 = _mm_shuffle_ps( ldata, ldata, _MM_SHUFFLE(3,0,2,1) );
    tmp1 = _mm_shuffle_ps( rdata, rdata, _MM_SHUFFLE(3,1,0,2) );
    tmp2 = _mm_shuffle_ps( ldata, ldata, _MM_SHUFFLE(3,1,0,2) );
    tmp3 = _mm_shuffle_ps( rdata, rdata, _MM_SHUFFLE(3,0,2,1) );
    qv = vec_mul( vec_splat( ldata, 3 ), rdata );
    qv = vec_madd( vec_splat( rdata, 3 ), ldata, qv );
    qv = vec_madd( tmp0, tmp1, qv );
    qv = vec_nmsub( tmp2, tmp3, qv );
    product = vec_mul( ldata, rdata );
    l_wxyz = vec_sld( ldata, ldata, 12 );
    r_wxyz = vec_sld( rdata, rdata, 12 );
    qw = vec_nmsub( l_wxyz, r_wxyz, product );
    xy = vec_madd( l_wxyz, r_wxyz, product );
    qw = vec_sub( qw, vec_sld( xy, xy, 8 ) );
	ALIGN_PROLOG(16) unsigned int sw[4] ALIGN_EPILOG(16) = {0, 0, 0, 0xffffffff};
    return Quat( vec_sel( qv, qw, sw ) );
}

inline Quat & Quat::operator *=( const Quat &quat )
{
    *this = *this * quat;
    return *this;
}

inline const Vector3 rotate( const Quat &quat, const Vector3 &vec )
{    __m128 qdata, vdata, product, tmp0, tmp1, tmp2, tmp3, wwww, qv, qw, res;
    qdata = quat.get128();
    vdata = vec.get128();
    tmp0 = _mm_shuffle_ps( qdata, qdata, _MM_SHUFFLE(3,0,2,1) );
    tmp1 = _mm_shuffle_ps( vdata, vdata, _MM_SHUFFLE(3,1,0,2) );
    tmp2 = _mm_shuffle_ps( qdata, qdata, _MM_SHUFFLE(3,1,0,2) );
    tmp3 = _mm_shuffle_ps( vdata, vdata, _MM_SHUFFLE(3,0,2,1) );
    wwww = vec_splat( qdata, 3 );
    qv = vec_mul( wwww, vdata );
    qv = vec_madd( tmp0, tmp1, qv );
    qv = vec_nmsub( tmp2, tmp3, qv );
    product = vec_mul( qdata, vdata );
    qw = vec_madd( vec_sld( qdata, qdata, 4 ), vec_sld( vdata, vdata, 4 ), product );
    qw = vec_add( vec_sld( product, product, 8 ), qw );
    tmp1 = _mm_shuffle_ps( qv, qv, _MM_SHUFFLE(3,1,0,2) );
    tmp3 = _mm_shuffle_ps( qv, qv, _MM_SHUFFLE(3,0,2,1) );
    res = vec_mul( vec_splat( qw, 0 ), qdata );
    res = vec_madd( wwww, qv, res );
    res = vec_madd( tmp0, tmp1, res );
    res = vec_nmsub( tmp2, tmp3, res );
    return Vector3( res );
}

inline const Quat conj( const Quat &quat )
{
	ALIGN_PROLOG(16) unsigned int sw[4] ALIGN_EPILOG(16) = {0x80000000,0x80000000,0x80000000,0};
    return Quat( vec_xor( quat.get128(), _mm_load_ps((float *)sw) ) );
}

inline const Quat select( const Quat &quat0, const Quat &quat1, bool select1 )
{
    return select( quat0, quat1, boolInVec(select1) );
}

inline const Quat select( const Quat &quat0, const Quat &quat1, const boolInVec &select1 )
{
    return Quat( vec_sel( quat0.get128(), quat1.get128(), select1.get128() ) );
}

inline bool equivalent ( const Quat & quat0, const Quat & quat1, float epsilon)
{
	__m128	abs		= fabsf4( _mm_sub_ps( quat0.get128(), quat1.get128() ) );	// Subtract and abs
	__m128	eps		= _mm_set1_ps(epsilon);									// Construct epsilon
	int		cmpmask = _mm_movemask_ps( _mm_cmple_ps(abs, eps) );			// Compose mask
	return	cmpmask == 0xf;
}

inline bool equivalentRelative(const Quat & quat0, const Quat & quat1, float precision)
{
	return m128_cmp_rel(quat0.get128(), quat1.get128(), precision, 4);
}

} // namespace VML

#endif
