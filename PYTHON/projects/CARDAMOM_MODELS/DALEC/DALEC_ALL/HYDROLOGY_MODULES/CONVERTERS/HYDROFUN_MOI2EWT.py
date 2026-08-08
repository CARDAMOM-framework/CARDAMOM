def HYDROFUN_MOI2EWT(moi, p, z):
    """Convert volumetric soil moisture (m3/m3) to equivalent water thickness (mm or kg/m2)"""
    return moi * 1000.0 * p * z
