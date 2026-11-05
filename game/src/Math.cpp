
RMAPI Vector2 Vector2ProjectPointLine(Vector2 P, Vector2 A, Vector2 B)
{
    Vector2 AB = (B - A);
    float t = Vector2DotProduct((P - A), AB) / Vector2DotProduct(AB, AB);
    return A + (AB * Clamp(t, 0.0f, 1.0f));
}

inline void NearestPointsOnLines(Vector2 bot1, Vector2 top1, Vector2 bot2, Vector2 top2, Vector2* nearest1, Vector2* nearest2)
{
    {
        Vector2 A = Vector2ProjectPointLine(top1, bot2, top2); // proj_top1_line2 
        Vector2 B = Vector2ProjectPointLine(bot1, bot2, top2); // proj_bot1_line2
        Vector2 C = Vector2ProjectPointLine(A, bot1, top1);    // proj_a_line1
        Vector2 D = Vector2ProjectPointLine(B, bot1, top1);    // proj_b_line1
        *nearest1 = Vector2LengthSqr(A - C) < Vector2LengthSqr(B - D) ? C : D; // Point on line 1 nearest to line 2
    }

    {
        Vector2 A = Vector2ProjectPointLine(top2, bot1, top1); // proj_top2_line1 
        Vector2 B = Vector2ProjectPointLine(bot2, bot1, top1); // proj_bot2_line1
        Vector2 C = Vector2ProjectPointLine(A, bot2, top2);    // proj_a_line2
        Vector2 D = Vector2ProjectPointLine(B, bot2, top2);    // proj_b_line2
        *nearest2 = Vector2LengthSqr(A - C) < Vector2LengthSqr(B - D) ? C : D; // Point on line 2 nearest to line 1
    }
}
