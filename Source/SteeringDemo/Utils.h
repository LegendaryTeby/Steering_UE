#pragma once

static FVector ClampVector(const FVector& _vector, const float _max)
{
	const float _length = _vector.Length();
	float _v = _length / _max;
	_v = _v < 1.0f ? _v : 1.0f;
	return _vector.GetSafeNormal() * (_v * _max);
}