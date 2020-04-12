/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_ProceduralGeometry_Cone.inl
	Internal generation functions for Cone.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_PROCEDURALGEOMETRY_C


a3ret a3proceduralCreateDescriptorCone(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3f32 length, a3ubyte slices, a3ubyte subdivsL, a3ubyte subdivsBaseRings)
{
	a3_ProceduralGeometryDescriptor ret = { 0 };
	if (geom_out && !geom_out->shape)
	{
		// validate params
		if (radius <= 0.0f)
			radius = 1.0f;
		if (length <= 0.0f)
			length = 0.5f;
		if (slices < 3)
			slices = 8;
		if (subdivsL < 1)
			subdivsL = 3;
		if (subdivsBaseRings < 1)
			subdivsBaseRings = 4;
		if (radius > 0.0f && length > 0.0f && slices >= 3 && subdivsL && subdivsBaseRings)
		{
			// store variables
			ret.fParams[fRadius] = radius;
			ret.fParams[fLength] = length;
			ret.bParams[bSlices] = slices;
			ret.bParams[bSubdivsL] = subdivsL;
			ret.bParams[bSubdivsB] = subdivsBaseRings;

			// create descriptor
			ret.shape = a3geomShape_cone;
			*geom_out = ret;
			return a3proceduralSetDescriptorFlagsAxis(geom_out, flags, axis);
		}
	}
	return -1;
}


a3ret a3proceduralInternalGenerateCone(a3_GeometryData *geomData, const a3_ProceduralGeometryDescriptor *geom)
{
	// recover params and flags
	const a3f32 radius = geom->fParams[fRadius];
	const a3f32 length = geom->fParams[fLength];
	const a3ui32 slices = geom->bParams[bSlices];
	const a3ui32 subdivsL = geom->bParams[bSubdivsL];
	const a3ui32 subdivsBase = geom->bParams[bSubdivsB];
	const a3_ProceduralGeometryAxis axis = a3proceduralGetDescriptorAxis(geom);
	const a3_ProceduralGeometryFlag flags = a3proceduralGetDescriptorFlags(geom);
	const a3boolean isSolid = (flags != a3geomFlag_wireframe);
	const a3_VertexPrimitiveType primType = geomData->primType = isSolid ? a3prim_triangles : a3prim_lineStrip;

	// get vertex count
	// **same algorithm as semisphere, just using subdivs instead of stacks
	const a3ui32 baseVerts = a3proceduralInternalCircleNumVertices(slices, subdivsBase, isSolid);
	const a3ui32 bodyVerts = isSolid ? (slices + 1)*subdivsL : slices*(subdivsL - 1);
	const a3ui32 capVerts = isSolid ? slices : 1;
	const a3ui32 baseInds = a3proceduralInternalCircleNumIndices(slices, subdivsBase, isSolid);
	const a3ui32 bodyInds = isSolid ? (slices)*(subdivsL - 1) * 6 : (slices + 1)*(subdivsL - 1);
	const a3ui32 capInds = isSolid ? slices * 3 : a3proceduralInternalCircleSpokeNumIndices(slices, subdivsL);
	const a3ui32 numVertices = geomData->numVertices = baseVerts + bodyVerts + capVerts;
	const a3ui32 numIndices = geomData->numIndices = baseInds + bodyInds + capInds;
	const a3ui32 vertexSize = a3proceduralInternalCreateFormats(geomData->vertexFormat, geomData->indexFormat, numVertices, numIndices, flags, 0);
	const a3ui32 indexSize = geomData->indexFormat->indexSize;


	// first tangent basis, to be transformed per vertex
	a3f32 t[3] = { 0.0f, 1.0f, 0.0f }, b[3] = { 0.0f, 0.0f, 1.0f }, n[3] = { 1.0f, 0.0f, 0.0f };
	a3f32 tBase[3] = { 1.0f, 0.0f, 0.0f }, bBase[3] = { 0.0f, -1.0f, 0.0f }, nBase[3] = { 0.0f, 0.0f, -1.0f };

	// allocate space for whole buffer required
	const a3ui32 vertexBufferSize = a3vertexFormatGetStorageSpaceRequired(geomData->vertexFormat, numVertices);
	const a3ui32 indexBufferSize = a3indexFormatGetStorageSpaceRequired(geomData->indexFormat, numIndices);
	a3ubyte *const data = (a3ubyte *)malloc(vertexBufferSize + indexBufferSize);

	// temporary pointers to non-interleaved attributes
	a3f32 *const positions = (a3f32 *)(data),
		*const texcoords = positions + geomData->vertexFormat->attribElements[a3attrib_position] * numVertices,
		*const normals = texcoords + geomData->vertexFormat->attribElements[a3attrib_texcoord] * numVertices,
		*const tangents = normals + geomData->vertexFormat->attribElements[a3attrib_normal] * numVertices,
		*const bitangents = tangents + geomData->vertexFormat->attribElements[a3attrib_tangent] * numVertices,
		*const vertexDataEnd = bitangents + geomData->vertexFormat->attribElements[a3attrib_bitangent] * numVertices;
	a3ubyte *const vertexDataEndConfirm = data + vertexBufferSize;
	a3ubyte *const indices = vertexDataEndConfirm,
		*const indexDataEnd = indices + indexSize * numIndices;
	a3ubyte *const indexDataEndConfirm = data + vertexBufferSize + indexBufferSize;
	a3f32 *attribItr = positions, *attribItr2 = positions, *attribItr3 = positions, *pPtr = positions, *tcPtr = texcoords;
	a3ubyte *indexItr = indices;
	a3ui32 i = 0, j = 0, k = 0;
	a3i32 indexDelta = 0;
	a3i64 valid = 0;

	// generate base everything
	const a3ui32 baseVertsOffset = baseVerts * vElems;
	const a3ui32 baseIndsOffset = baseInds * indexSize;


	//-------------------------------------------------------------------------
	// solid
	if (isSolid)
	{
		// **same algorithm as semisphere, use length and subdivs instead of elevation stacks
		const a3f32 dU = 1.0f / (a3f32)slices;
		const a3f32 dV = 0.5f / (a3f32)subdivsL;
		const a3f32 deltaLen = length / (a3f32)subdivsL;
		const a3f32 deltaAzim = 360.0f / (a3f32)slices;
		a3f32 elev = 0.0f, ringRadius = 0.0f;

		// base: using algorithm
		a3proceduralInternalCircleGenerateAlgorithm(
			positions,
			(flags & a3geomFlag_texcoords ? texcoords : 0),
			(flags & a3geomFlag_normals ? normals : 0),
			(flags & a3geomFlag_tangentFlag ? tangents : 0),
			(flags & a3geomFlag_tangentFlag ? bitangents : 0),
			indices,
			radius, slices, subdivsBase, 0.0f, 0.0f, 0.0f, 0.75f, 0.25f, 0.25f, baseVerts, baseInds, indexSize, 0, isSolid, 1
		);

		// offset pointers
		attribItr = positions + baseVertsOffset;

		// body: rings, extra around base for different attributes
		attribItr = a3proceduralInternalCreateCircle(attribItr, radius, 0.0f, 0.0f, 0.0f, 0.0f, slices, vElems, 1, 0);
		for (i = 1, elev = deltaLen; i < subdivsL; elev = deltaLen*(a3f32)(++i))
		{
			ringRadius = (length - elev) / length;
			attribItr = a3proceduralInternalCreateCircle(attribItr, radius * ringRadius, 0.0f, 0.0f, elev, 0.0f, slices, vElems, 1, 0);
		}
	
		// cap: create one point at the pole per slice
		for (i = 0; i < slices; ++i, attribItr += vElems)
		{
			attribItr[0] = attribItr[1] = 0.0f;
			attribItr[2] = length;
		}


		// done, transform AT THE END because current state used for normals
		assert(valid = attribItr == texcoords);

		// indices
		indexItr = indices + baseIndsOffset;

		// body is a grid, then cap, then base
		i = baseVerts;
		indexItr = a3proceduralInternalCreatePlaneIndicesSolid(indexItr, slices, subdivsL - 1, indexSize, &i, 1, 0);
		// i now points to the cap vertex start
		indexItr = a3proceduralInternalStoreCircleCapIndicesSolid(indexItr, indexSize, slices, i, i - (slices + 1), 0);
		assert(valid = indexItr == indexDataEnd);


		// texcoords
		if (flags & a3geomFlag_texcoords)
		{
			// body: grid, starting halfway up in texture space
			// need to divide half of texture space evenly
			attribItr = texcoords + baseVerts * tElems;
			attribItr = a3proceduralInternalCreatePlane(attribItr, 1.0f, 0.0f, 1.0f - dV, 0.5f, 0.0f, slices, subdivsL - 1, tElems, 1);

			// cap: create one point at the pole per slice
			for (i = 0; i < slices; ++i, attribItr += tElems)
			{
				attribItr[0] = dU*((a3f32)i + 0.5f);
				attribItr[1] = 1.0f;
			}

			assert(valid = attribItr == normals);

			geomData->attribData[a3attrib_geomTexcoord] = texcoords;
		}

		// normals
		if (flags & a3geomFlag_normals)
		{
			// normals are easy: normalize the position
			// ...but we need the length; hard-code the first 
			//	triangle to calculate correct vector length
			const a3ui32 bodyVerts = (slices + 1)*subdivsL;
			const a3ui32 capVerts = slices;
			const a3f32 p0[3] = {
				radius, 0.0f, 0.0f
			}, p1[3] = {
				radius*a3cosdTaylor(deltaAzim), radius*a3sindTaylor(deltaAzim), 0.0f
			}, p2[3] = {
				radius*deltaLen / length, 0.0f, deltaLen
			}, tc0[2] = {
				0.0f, 0.5f
			}, tc1[2] = {
				dU, 0.5f
			}, tc2[2] = {
				0.0f, 0.5f + dV
			};
			a3f32 tLen = 0.0f, bLen = 0.0f, nLen = 0.0f;
			a3f32 lenConvert;
			a3proceduralInternalCalculateTangentBasis(t, b, n, p0, p1, p2, tc0, tc1, tc2);
			tLen = a3real3Length(t);
			bLen = a3real3Length(b);
			nLen = a3real3Length(n);

			// normal is perpendicular to edge; calculate once per edge and copy all the way up
			attribItr = attribItr2 = normals + baseVertsOffset, pPtr = positions + baseVertsOffset;
			// body: calculate perpendicular vector for each slice
			for (i = 0; i <= slices; ++i, attribItr += vElems, attribItr2 = attribItr, pPtr += vElems)
			{
				// copy x and y, z is perpendicular to face
				attribItr[0] = pPtr[0];
				attribItr[1] = pPtr[1];
				attribItr[2] = radius * radius / length;
				lenConvert = nLen / a3real3Length(attribItr);
			//	lenConvert = 1.0f / a3real3Length(attribItr);
				attribItr[0] *= lenConvert;
				attribItr[1] *= lenConvert;
				attribItr[2] *= lenConvert;
				for (j = 1; j < subdivsL; ++j)
				{
					attribItr2 += (slices + 1) * vElems;
					attribItr2[0] = attribItr[0];
					attribItr2[1] = attribItr[1];
					attribItr2[2] = attribItr[2];
				}
			}
			// cap: copy face normal calculated
			attribItr = normals + baseVertsOffset;
			attribItr2 = attribItr + bodyVerts* vElems;
			attribItr3 = attribItr + vElems;
			for (i = 0; i < capVerts; ++i, attribItr += vElems, attribItr2 += vElems, attribItr3 += vElems)
			{
				attribItr2[0] = 0.5f * (attribItr[0] + attribItr3[0]);
				attribItr2[1] = 0.5f * (attribItr[1] + attribItr3[1]);
				attribItr2[2] = 0.5f * (attribItr[2] + attribItr3[2]);
			}

			assert(valid = attribItr2 == tangents);

			geomData->attribData[a3attrib_geomNormal] = normals;

			if (flags & a3geomFlag_tangentFlag)
			{
				attribItr = tangents + baseVertsOffset;
				// create body tangents once and copy to all stacks
				// just do a circle with a 90 degree offset
				lenConvert = tLen;
				//	lenConvert = 1.0f;
				attribItr2 = a3proceduralInternalCreateCircle(attribItr, lenConvert, 0.0f, 0.0f, 0.0f, 90.0f, slices, vElems, 1, 0);
				for (i = 1; i < subdivsL; ++i)
					for (j = 0, attribItr3 = attribItr; j <= slices; ++j, attribItr2 += vElems, attribItr3 += vElems)
					{
						attribItr2[0] = attribItr3[0];
						attribItr2[1] = attribItr3[1];
						attribItr2[2] = attribItr3[2];
					}
				// cap (less than not equal this time because there's no repeat)
				for (j = 0, attribItr3 = attribItr; j < slices; ++j, attribItr2 += vElems, attribItr3 += vElems)
				{
					attribItr2[0] = attribItr3[0];
					attribItr2[1] = attribItr3[1];
					attribItr2[2] = attribItr3[2];
				}

				assert(valid = attribItr2 == bitangents);

				// bitangents: all at once
				attribItr = tangents + baseVertsOffset;
				attribItr2 = bitangents + baseVertsOffset;
				attribItr3 = normals + baseVertsOffset;
				for (i = 0, j = bodyVerts + capVerts; i < j; ++i, attribItr += vElems, attribItr2 += vElems, attribItr3 += vElems)
					a3real3Cross(attribItr2, attribItr3, attribItr);

				// done
				assert(valid = attribItr == bitangents);
				assert(valid = attribItr2 == vertexDataEnd);

				geomData->attribData[a3attrib_geomTangent] = tangents;

				// transform tangents
				a3proceduralInternalTransformVectors(tangents, numVertices, axis);
				a3proceduralInternalTransformVectors(bitangents, numVertices, axis);
			}

			// transform normals
			a3proceduralInternalTransformVectors(normals, numVertices, axis);
		}

		// transform positions
		a3proceduralInternalTransformVectors(positions, numVertices, axis);
	}


	//-------------------------------------------------------------------------
	// wireframe
	else
	{
		// body: same as above but without repeating end
		const a3f32 deltaLen = length / (a3f32)subdivsL;
		a3f32 elev = 0.0f, ringRadius = 0.0f;

		// using algo
		a3proceduralInternalCircleGenerateAlgorithm(positions, 0, 0, 0, 0, indices,
			radius, slices, subdivsBase, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, baseVerts, baseInds, indexSize, 0, isSolid, 0
		);

		// offset
		attribItr = positions + baseVertsOffset;

		// body, no extra ring
		for (i = 1, elev = deltaLen; i < subdivsL; elev = deltaLen*(a3f32)(++i))
		{
			ringRadius = (length - elev) / length;
			attribItr = a3proceduralInternalCreateCircle(attribItr, radius * ringRadius, 0.0f, 0.0f, elev, 0.0f, slices, vElems, 0, 0);
		}

		// add exactly one vertex for cap
		attribItr[0] = attribItr[1] = 0.0f;
		attribItr[2] = length;
		attribItr += vElems;

		assert(valid = attribItr == texcoords);

		// indices: 
		indexItr = indices + baseIndsOffset;

		// body: closed loops
		for (j = 1, i = baseVerts; j < subdivsL; ++j, i += slices)
			indexItr = a3proceduralInternalStoreRingIndices(indexItr, indexSize, slices + 1, i, 1);

		// spokes
		indexItr = a3proceduralInternalStoreCircleIndicesWireSpokes(indexItr, indexSize, slices, subdivsL, numVertices - 1, numVertices - 1 - slices, 0, 0, +1, +1, 1);

		assert(valid = indexItr == indexDataEnd);

		// transform positions
		a3proceduralInternalTransformVectors(positions, numVertices, axis);
	}


	//-------------------------------------------------------------------------

	// copy remaining stuff
	geomData->attribData[a3attrib_geomPosition] = positions;
	geomData->indexData = numIndices ? indices : 0;
	geomData->data = data;


	//-------------------------------------------------------------------------
	// done
	return 1;
}


#endif	// __ANIMAL3D_PROCEDURALGEOMETRY_C